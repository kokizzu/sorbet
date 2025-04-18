#include "rewriter/TypeAssertion.h"
#include "ast/Helpers.h"
#include "ast/ast.h"
#include "core/core.h"

namespace sorbet::rewriter {

ast::ExpressionPtr TypeAssertion::run(core::MutableContext ctx, ast::Send *send) {
    if (!ast::MK::isT(send->recv)) {
        return nullptr;
    }

    switch (send->fun.rawId()) {
        case core::Names::let().rawId():
        case core::Names::bind().rawId():
        case core::Names::uncheckedLet().rawId():
        case core::Names::assertType().rawId():
        case core::Names::cast().rawId():
            break;
        default:
            return nullptr;
    }

    if (send->numPosArgs() < 2 || send->hasBlock()) {
        return nullptr;
    }

    auto expr = std::move(send->getPosArg(0));
    auto typeExpr = std::move(send->getPosArg(1));

    return ast::make_expression<ast::Cast>(send->loc, core::Types::todo(), std::move(expr), send->fun,
                                           std::move(typeExpr));
}

} // namespace sorbet::rewriter
