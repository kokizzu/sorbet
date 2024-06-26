# typed: true
extend T::Sig

sig { returns(String) }
def syntactic_lambda
  f = ->() {
    return 0
  }
  f.call.to_s
end

sig { returns(String) }
def kernel_lambda
  f = Kernel.lambda {
    return 0
  }
  f.call.to_s
end

sig { returns(String) }
def plain_lambda
  f = lambda {
    return 0 # error: Expected `String` but found `Integer(0)` for method result type
  }
  f.call.to_s
end

sig { returns(String) }
def proc_to_lambda
  p = proc {
    return 0 # error: Expected `String` but found `Integer(0)` for method result type
  }
  f = Kernel.lambda(&p)
  f.call.to_s
end

sig { params(blk: T.proc.returns(NilClass)).void }
def takes_nil_block(&blk)
  yield
end

sig { returns(String) }
def block_inside_lambda
  f = ->() {
    takes_nil_block do
      # At runtime, this returns from the lambda, but Sorbet treats it like it
      # returns from the enclosing block. This would be nice to fix, because
      # there is no workaround for this except manually raising and catching
      # exceptions.
      return 0 # error: Expected `NilClass` but found `Integer(0)` for block result type
    end
  }
  f.call.to_s
end
