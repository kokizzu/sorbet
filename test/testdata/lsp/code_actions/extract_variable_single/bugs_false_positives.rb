# typed: true
# selective-apply-code-action: refactor.extract
# enable-experimental-lsp-extract-to-variable: true
#
# This file contain cases where we should disallow the user from extracting to variable,
# but incorrectly allow it.

class A < T::Struct
  prop :x, Integer
#      ^^ apply-code-action: [A] Extract Variable (this occurrence only)
  prop :y, String
#          ^^^^^^ apply-code-action: [B] Extract Variable (this occurrence only)
end
