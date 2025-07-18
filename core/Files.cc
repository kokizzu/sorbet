#include "core/Files.h"
#include "common/FileOps.h"
#include "core/Context.h"
#include "core/FileHash.h"
#include "core/GlobalState.h"
#include "core/SigilTraits.h"
#include <vector>

#include "absl/strings/match.h"

template class std::vector<std::shared_ptr<sorbet::core::File>>;
template class std::shared_ptr<sorbet::core::File>;
using namespace std;

namespace sorbet::core {

namespace {

constexpr auto EXTERNAL_PREFIX = "external/com_stripe_ruby_typer/"sv;

template <class Sigil> class ParseSigil {
public:
    static Sigil parse(string_view source) {
        size_t start = 0;
        while (true) {
            start = source.find(SigilTraits<Sigil>::SIGIL_PREFIX, start);
            if (start == string_view::npos) {
                return SigilTraits<Sigil>::NONE;
            }

            auto comment_start = start;
            while (comment_start > 0) {
                --comment_start;
                auto c = source[comment_start];
                if (c == ' ') {
                    continue;
                } else {
                    break;
                }
            }
            if (source[comment_start] != '#') {
                ++start;
                continue;
            }

            start += SigilTraits<Sigil>::SIGIL_PREFIX.size();
            while (start < source.size() && source[start] == ' ') {
                ++start;
            }

            if (start >= source.size()) {
                return SigilTraits<Sigil>::NONE;
            }
            auto end = start + 1;
            while (end < source.size() && source[end] != ' ' && source[end] != '\n') {
                ++end;
            }
            if (source[end - 1] == '\r') {
                end -= 1;
            }

            string_view suffix = source.substr(start, end - start);
            auto result = SigilTraits<Sigil>::fromString(suffix);
            if (result != SigilTraits<Sigil>::NONE) {
                return result;
            } else {
                // TODO(nelhage): We should report an error here to help catch
                // typos. This would require refactoring so this function has
                // access to GlobalState or can return errors to someone who
                // does.
            }

            start = end;
        }
    }
};

} // namespace

StrictLevel File::fileStrictSigil(string_view source) {
    return ParseSigil<StrictLevel>::parse(source);
}

PackagedLevel File::filePackagedSigil(string_view source) {
    return ParseSigil<PackagedLevel>::parse(source);
}

bool isTestPath(string_view path) {
    return absl::EndsWith(path, ".test.rb") || absl::StrContains(path, "/test/");
}

bool isTestHelperPath(string_view path) {
    return absl::StrContains(path, "/test/") && !absl::EndsWith(path, ".test.rb");
}

bool isPackageRBIPath(string_view path) {
    return absl::EndsWith(path, ".package.rbi");
}

bool File::isRBIPath(string_view path) {
    return absl::EndsWith(path, ".rbi");
}

bool File::isPackagePath(string_view path) {
    auto pos = path.rfind("/");
    if (pos != string_view::npos) {
        path = path.substr(pos + 1);
    }

    return path == "__package.rb";
}

File::Flags::Flags(string_view path)
    : hasIndexErrors(false), isPackagedTestHelper(isTestHelperPath(path)), isPackagedTest(isTestPath(path)),
      hasPackageRBIPath(isPackageRBIPath(path)), hasPackageRbPath(isPackagePath(path)), isOpenInClient(false) {}

File::File(string &&path_, string &&source_, Type sourceType, uint32_t epoch)
    : epoch(epoch), sourceType(sourceType), flags(path_), packagedLevel{File::filePackagedSigil(source_)},
      path_(move(path_)), source_(move(source_)), originalSigil(fileStrictSigil(this->source_)),
      strictLevel(originalSigil) {
    if (this->source_.size() >= INVALID_POS_LOC) [[unlikely]] {
        Exception::raise("File not less than {} bytes. Got: {}", UINT32_MAX, this->source_.size());
    }
}

unique_ptr<File> File::deepCopy(GlobalState &gs) const {
    string sourceCopy = source_;
    string pathCopy = path_;
    auto ret = make_unique<File>(move(pathCopy), move(sourceCopy), sourceType, epoch);
    ret->lineBreaks_ = lineBreaks_;
    ret->minErrorLevel_ = minErrorLevel_;
    ret->strictLevel = strictLevel;
    return ret;
}

void File::setFileHash(unique_ptr<const FileHash> hash) {
    // If hash_ != nullptr, then the contents of hash_ and hash should be identical.
    // Avoid needlessly invalidating references to *hash_.
    if (hash_ == nullptr) {
        hash_ = move(hash);
    }
}

const shared_ptr<const FileHash> &File::getFileHash() const {
    return hash_;
}

FileRef::FileRef(unsigned int id) : _id(id) {}

const File &FileRef::data(const GlobalState &gs) const {
    ENFORCE(gs.files[_id]);
    ENFORCE(gs.files[_id]->sourceType != File::Type::TombStone);
    ENFORCE(gs.files[_id]->sourceType != File::Type::NotYetRead);
    return dataAllowingUnsafe(gs);
}

File &FileRef::data(GlobalState &gs) const {
    ENFORCE(gs.files[_id]);
    ENFORCE(gs.files[_id]->sourceType != File::Type::TombStone);
    ENFORCE(gs.files[_id]->sourceType != File::Type::NotYetRead);
    return dataAllowingUnsafe(gs);
}

const File &FileRef::dataAllowingUnsafe(const GlobalState &gs) const {
    ENFORCE(_id < gs.filesUsed());
    return *(gs.files[_id]);
}

File &FileRef::dataAllowingUnsafe(GlobalState &gs) const {
    ENFORCE(_id < gs.filesUsed());
    return *(gs.files[_id]);
}

bool FileRef::isPackage(const GlobalState &gs) const {
    ENFORCE(gs.files[_id]);
    ENFORCE(gs.files[_id]->sourceType != File::Type::TombStone);
    return dataAllowingUnsafe(gs).isPackage(gs);
}

string_view File::path() const {
    return this->path_;
}

string_view File::source() const {
    ENFORCE(this->sourceType != File::Type::TombStone);
    ENFORCE(this->sourceType != File::Type::NotYetRead);
    return this->source_;
}

StrictLevel File::minErrorLevel() const {
    return minErrorLevel_;
}

bool File::isPayload() const {
    return sourceType == File::Type::PayloadGeneration || sourceType == File::Type::Payload;
}

bool File::isRBI() const {
    return File::isRBIPath(path());
}

bool File::isStdlib() const {
    return this->originalSigil == StrictLevel::Stdlib;
}

namespace {

constexpr string_view OVERLOADS_TEST_RB = "overloads_test.rb"sv;

}

bool File::permitOverloadDefinitions() const {
    return this->isRBI() || FileOps::getFileName(this->path()) == OVERLOADS_TEST_RB || this->isStdlib();
}

bool File::hasPackageRbPath() const {
    return flags.hasPackageRbPath && this->strictLevel != StrictLevel::Ignore;
}

bool File::isPackage(const GlobalState &gs) const {
    // If the `__package.rb` file is at `typed: ignore`, then we haven't even parsed it.
    // Any loop over "all package files" really only wants "all non-ignored package files."
    //
    // Checks `packageDB()` last because probably we have better locality on the `flags` for the
    // common case of this not being a `__package.rb` file.
    return hasPackageRbPath() && gs.packageDB().enabled();
}

bool File::isOpenInClient() const {
    return this->flags.isOpenInClient;
}

void File::setIsOpenInClient(bool isOpenInClient) {
    this->flags.isOpenInClient = isOpenInClient;
}

absl::Span<const uint32_t> File::lineBreaks() const {
    ENFORCE(this->sourceType != File::Type::TombStone);
    ENFORCE(this->sourceType != File::Type::NotYetRead);
    auto ptr = atomic_load(&lineBreaks_);
    if (ptr != nullptr) {
        return absl::MakeSpan(*ptr);
    } else {
        auto my = make_shared<vector<uint32_t>>(findLineBreaks(this->source_));
        atomic_compare_exchange_weak(&lineBreaks_, &ptr, my);
        return lineBreaks();
    }
}

int File::lineCount() const {
    return lineBreaks().size();
}

// 1-indexed line number
string_view File::getLine(int i) const {
    auto lineBreaks = this->lineBreaks();
    ENFORCE(i <= lineBreaks.size());
    ENFORCE(i > 0);
    auto start = i == 1 ? 0 : lineBreaks[i - 2] + 1;
    auto end = lineBreaks[i - 1];
    return source().substr(start, end - start);
}

string File::censorFilePathForSnapshotTests(string_view orig) {
    string_view result = orig;
    if (absl::StartsWith(result, EXTERNAL_PREFIX)) {
        // When running tests from outside of the sorbet repo, the files have a different path in the sandbox.
        result.remove_prefix(EXTERNAL_PREFIX.size());
    }

    if (absl::StartsWith(result, URL_PREFIX)) {
        // This is so that changing RBIs doesn't mean invalidating every symbol-table exp test.
        result.remove_prefix(URL_PREFIX.size());
        if (absl::StartsWith(result, EXTERNAL_PREFIX)) {
            result.remove_prefix(EXTERNAL_PREFIX.size());
        }
    }

    if (absl::StartsWith(orig, URL_PREFIX)) {
        return fmt::format("{}{}", URL_PREFIX, result);
    } else {
        return string(result);
    }
}

bool File::isPackagedTest() const {
    return flags.isPackagedTest;
}

bool File::isPackagedTestHelper() const {
    return flags.isPackagedTestHelper;
}

bool File::isPackageRBI() const {
    return flags.hasPackageRBIPath;
}

bool File::hasIndexErrors() const {
    return flags.hasIndexErrors;
}

void File::setHasIndexErrors(bool value) {
    flags.hasIndexErrors = value;
}

bool File::isPackaged() const {
    switch (this->packagedLevel) {
        case PackagedLevel::False:
            return false;

        case PackagedLevel::True:
        case PackagedLevel::None:
            return true;
    }
}

} // namespace sorbet::core
