#ifndef RUBY_TYPER_PIPELINE_H
#define RUBY_TYPER_PIPELINE_H
#include "ProgressIndicator.h"
#include "ast/ast.h"
#include "common/common.h"
#include "common/concurrency/WorkerPool.h"
#include "common/kvstore/KeyValueStore.h"
#include "core/FileHash.h"
#include "main/options/options.h"

namespace sorbet::core::lsp {
class PreemptionTaskManager;
}

namespace sorbet::realmain::pipeline {

// Copy any relevant options from the options to GlobalState
//
// Note that not all options show up in GlobalState, because some options are meant to only be used
// by the realmain/orchestration code, and not be possible to branch on by individual parts of the
// pipeline (for example: the `runLSP` boolean does not make it into GlobalState, so it's not
// possible to branch on whether we're in LSP or not when implementing a feature).
void setGlobalStateOptions(core::GlobalState &gs, const options::Options &opts);

std::vector<core::FileRef> reserveFiles(core::GlobalState &gs, const std::vector<std::string> &files);

// ----- indexer --------------------------------------------------------------

core::StrictLevel decideStrictLevel(const core::GlobalState &gs, const core::FileRef file,
                                    const options::Options &opts);

// Primarily exposed for LSP—outside of LSP, you probably want `indexOne`.
ast::ExpressionPtr desugarOne(const options::Options &opts, core::GlobalState &gs, core::FileRef file,
                              bool preserveConcreteSyntax);

// Run all of Sorbet's indexer phases (parsing, desugaring, rewriting, etc.) on a single file
ast::ParsedFile indexOne(const options::Options &opts, core::GlobalState &lgs, core::FileRef file,
                         ast::ExpressionPtr cachedTree = nullptr);

// Run all of Sorbet's indexer phases (parsing, desugaring, rewriting, etc.) over multiple files, in parallel
ast::ParsedFilesOrCancelled index(core::GlobalState &gs, absl::Span<const core::FileRef> files,
                                  const options::Options &opts, WorkerPool &workers,
                                  const std::unique_ptr<const OwnedKeyValueStore> &kvstore, bool cancelable = false);

// ----- packager -------------------------------------------------------------

size_t partitionPackageFiles(const core::GlobalState &gs, absl::Span<core::FileRef> files);
void unpartitionPackageFiles(std::vector<ast::ParsedFile> &packageFiles,
                             std::vector<ast::ParsedFile> &&nonPackageFiles);

void package(core::GlobalState &gs, absl::Span<ast::ParsedFile> what, const options::Options &opts,
             WorkerPool &workers);

void buildPackageDB(core::GlobalState &gs, absl::Span<ast::ParsedFile> what, const options::Options &opts,
                    WorkerPool &workers);

// Associates source files with their package in the package DB. This can only be called after `buildPackageDB`.
void setPackageForSourceFiles(core::GlobalState &gs, absl::Span<core::FileRef> sourceFiles,
                              const options::Options &opts);

void validatePackagedFiles(core::GlobalState &gs, absl::Span<ast::ParsedFile> what, const options::Options &opts,
                           WorkerPool &workers);

// ----- namer + resolver -----------------------------------------------------

[[nodiscard]] bool name(core::GlobalState &gs, absl::Span<ast::ParsedFile> what, const options::Options &opts,
                        WorkerPool &workers, core::FoundDefHashes *foundHashes);

ast::ParsedFilesOrCancelled resolve(core::GlobalState &gs, std::vector<ast::ParsedFile> what,
                                    const options::Options &opts, WorkerPool &workers);

ast::ParsedFilesOrCancelled nameAndResolve(core::GlobalState &gs, std::vector<ast::ParsedFile> what,
                                           const options::Options &opts, WorkerPool &workers,
                                           core::FoundDefHashes *foundHashes);

// If `foundMethodHashesForFiles` is non-nullopt, incrementalResolve invokes Namer in runIncremental mode.
//
// This is most useful when running incrementalResolve for the purpose of a file update.
//
// It's not required when running incrementalResolve just to turn an AST into a resolved AST, if
// that AST has already been resolved once before on the fast path
std::vector<ast::ParsedFile> incrementalResolve(
    core::GlobalState &gs, std::vector<ast::ParsedFile> what,
    std::optional<UnorderedMap<core::FileRef, std::shared_ptr<const core::FileHash>>> &&foundHashesForFiles,
    const options::Options &opts, WorkerPool &workers);

// ----- typecheck ------------------------------------------------------------

// Note: `cancelable` and `preemption task manager` are only applicable to LSP.
// If `intentionallyLeakASTs` is `true`, typecheck will leak the ASTs rather than pay the cost of deleting them
// properly, which is a significant speedup on large codebases.
void typecheck(const core::GlobalState &gs, std::vector<ast::ParsedFile> what, const options::Options &opts,
               WorkerPool &workers, bool cancelable = false,
               std::optional<std::shared_ptr<core::lsp::PreemptionTaskManager>> preemptionManager = std::nullopt,
               bool presorted = false, bool intentionallyLeakASTs = false);

// ----- other ----------------------------------------------------------------

void printFileTable(core::GlobalState &gs, const options::Options &opts, const UnorderedMap<long, long> &untypedUsages);

void printUntypedBlames(const core::GlobalState &gs, const UnorderedMap<long, long> &untypedBlames,
                        const options::Options &opts);

// Create a copy of `from` that has its symbol table reset to the payload.
std::unique_ptr<core::GlobalState> copyForSlowPath(const core::GlobalState &from, const options::Options &opts);

} // namespace sorbet::realmain::pipeline
#endif // RUBY_TYPER_PIPELINE_H
