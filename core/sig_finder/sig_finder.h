#ifndef SORBET_SIG_FINDER
#define SORBET_SIG_FINDER
#include "core/core.h"
#include "resolver/type_syntax/type_syntax.h"

namespace sorbet::sig_finder {

class SigFinder {
public:
    struct Result {
        resolver::ParsedSig sig;
        const ast::Send &origSend;
        Result(resolver::ParsedSig &&sig, const ast::Send &origSend) : sig(std::move(sig)), origSend(origSend) {}
    };

private:
    const core::Loc queryLoc;

    // Track the narrowest location range that still contains the queryLoc.
    //
    // If we find a method that's after queryLoc but it's not in this narrowest range,
    // it means we found a sig that's outside the scope where the queryLoc was.
    core::Loc narrowestClassDefRange;

    // Track whether the current scope has the queryLoc.
    std::vector<bool> scopeContainsQueryLoc;

    std::optional<Result> result_;

public:
    SigFinder(core::Loc queryLoc)
        : queryLoc(queryLoc), narrowestClassDefRange(core::Loc::none()), scopeContainsQueryLoc(std::vector<bool>{}),
          result_(std::nullopt) {}

    void preTransformClassDef(core::Context ctx, const ast::ClassDef &tree);
    void postTransformClassDef(core::Context ctx, const ast::ClassDef &tree);
    void preTransformMethodDef(core::Context ctx, const ast::MethodDef &tree);
    void postTransformRuntimeMethodDefinition(core::Context ctx, const ast::RuntimeMethodDefinition &tree);
    void preTransformSend(core::Context ctx, const ast::Send &tree);

    static std::optional<Result> findSignature(core::Context ctx, const ast::ExpressionPtr &tree, core::Loc queryLoc);
};

} // namespace sorbet::sig_finder

#endif // SORBET_SIG_FINDER
