# typed: strict
# enable-experimental-rbs-signatures: true
# enable-experimental-rbs-assertions: true

while ARGV.any?
  next ARGV.shift #: as String
end

while ARGV.any?
  next ARGV.shift, "foo" #: Array[String]
end

while ARGV.shift
  next #: as String
  #    ^^^^^^^^^^^^ error: Unexpected RBS assertion comment found after `next`
end
