---
id: cli-ref
title: Command Line Reference
sidebar_label: CLI Reference
---

<!-- DO NOT EDIT. This file is autogenerated.    -->
<!--                                             -->
<!-- To update the help output, run              -->
<!--                                             -->
<!--     bazel run //website:update_cli_ref      -->
<!--                                             -->
<!-- To update the preamble or formatting, edit  -->
<!--                                             -->
<!--     website/scripts/generate_cli_ref.sh     -->
<!--                                             -->
<!-- and then run the above command.             -->

This page shows the `srb tc --help` output for the latest version of Sorbet.

- See [Command Line Quickstart](cli.md) for an overview of some of the more
  common options and workflows.

- See `srb tc --help` to see the help options for your version of Sorbet. (New
  options are added and changed all the time.)

## Usage

```plaintext
Sorbet: A fast, powerful typechecker designed for Ruby
Usage:
  sorbet [options] [[--] <path>...]

```

## INPUT options

```plaintext
  -e <string>                   Treat <string> as if it were the contents of a Ruby file
                                passed on the command line (default: "")
      --e-rbi <string>          Like `-e`, but treat <string> as an RBI file (default:
                                "")
      --file <path>             Run over the contents of <path>
                                (Equivalent to passing <path> as a positional argument)
      --dir <path>              Run over all Ruby and RBI files in <path>, recursively
                                (Equivalent to passing <path> as a positional argument)
      --allowed-extension <ext>[,<ext>...]
                                Use these extensions to determine which file types Sorbet
                                should discover inside directories. (default: .rb,.rbi)
      --ignore <pattern>        Ignores input files that contain <pattern> in their paths
                                (relative to the input path passed to Sorbet).
                                When <pattern> starts with `/` it matches against the
                                prefix of these relative paths; others match anywhere.
                                Matches must be against whole path segments, so `foo`
                                matches `foo/bar.rb` and `bar/foo/baz.rb` but not
                                `foo.rb` or `foo2/bar.rb`.
      --no-config               Do not load the content of the `sorbet/config` file.
                                Otherwise, Sorbet reads the `sorbet/config` file and
                                treats each line as if it were passed on the command
                                line, unless the line starts with `#`.
                                To load a <file> as if it were a config file, pass
                                `@<file>` as a positional arg
      --typed {false,true,strict,strong,[auto]}
                                Force all code to specified strictness level,
                                disregarding all `# typed:` sigils. For `auto`, uses the
                                `# typed:` sigil in the file or `false` for files without
                                a sigil. (default: auto)
      --typed-override <filepath.yaml>
                                Read <filepath.yaml> to override the strictness of
                                individual files.
                                Contents must be a map of `<strictness>: ['path1.rb',
                                ...]` pairs.
                                Can be used to enable type checking for certain files
                                temporarily without having to add a comment to every
                                file. (default: "")

```

## OUTPUT options

```plaintext
  -q, --quiet                   Silence all non-critical errors
  -P, --progress                Draw progressbar
      --color {always,never,[auto]}
                                Use color output. For `auto`: use color if stderr is a
                                tty (default: auto)
      --no-error-count          Do not print the `Errors: <N>` summary line
      --no-error-sections       Only print the first line of every error message
                                (suppress any additional information below an error). Can
                                provide substantial speedups when dealing with many
                                errors
      --error-url-base <url-base>
                                Every error message includes a link created by prefixing
                                that error's code with <url-base>. Can be used to
                                maintain docs on Sorbet error codes which are more
                                relevant to a specific project or company. (default:
                                https://srb.help/)
      --remove-path-prefix <prefix>
                                Remove the provided <prefix> from all printed paths.
                                Defaults to the input directory passed to Sorbet, if any.
                                (default: "")

```

## AUTOCORRECT options

```plaintext
  -a, --autocorrect             Auto-correct source files with suggested fixes.
                                Use the `--{isolate,suppress}-error-code` options to
                                control which corrections to apply
      --suggest-unsafe [=<method>(=T.unsafe)]
                                Include 'unsafe' autocorrects, e.g. those which can be
                                fixed by wrapping code in `T.unsafe` or using
                                `T.untyped`. Provide a custom <method> to wrap with
                                `<method>(...)` instead of `T.unsafe(...)`. This
                                supersedes certain autocorrects, especially T.must.
      --did-you-mean            Whether to include 'Did you mean' suggestions in
                                autocorrects. For large codemods, it's usually better to
                                avoid spurious changes by setting this to false (default:
                                true)
      --suggest-typed           Emit autocorrects to set the `# typed:` sigil in every
                                file to the highest possible level where no errors would
                                be reported in that file. Will downgrade the `# typed:`
                                sigil for any files with errors.

```

## ERROR options

```plaintext
      --typed-super             Enable typechecking of `super` calls when possible
                                (default: true)
      --check-out-of-order-constant-references
                                Detect when a constant is referenced early in a file, but
                                defined later in that file. Does not detect out-of-order
                                references across file boundaries.
      --isolate-error-code <error-code>
                                Which error(s) to include in reporting. This option can
                                be passed multiple times. All errors not mentioned will
                                be silenced.
      --suppress-error-code <error-code>
                                Which error(s) to exclude from reporting. This option can
                                be passed multiple times.
      --suppress-payload-superclass-redefinition-for Fully::Qualified::ClassName
                                Explicitly suppress the superclass redefinition error for
                                the specified class defined in Sorbet's payload. May be
                                repeated.
      --experimental-ruby3-keyword-args
                                Enforce use of new (Ruby 3.0-style) keyword arguments.
                                (incomplete and experimental)
      --enable-experimental-requires-ancestor
                                Enable experimental `requires_ancestor` annotation
      --stripe-mode             Ensure that every class and module only defines
                                'behavior' in one file. Ensures that every class or
                                module can be autoloaded by loading exactly one file.
      --stop-after <phase>      Stop after completing <phase>. Can be useful for
                                debugging. Also useful when overwhelmed with errors,
                                because errors from earlier phases (like resolver) can
                                cause errors downstream (in inferencer).
                                Phases: [init, parser, desugarer, rewriter, local-vars,
                                namer, resolver, cfg, inferencer] (default: inferencer)

```

## METRIC options

```plaintext
      --counters                Print all internal counters
      --counter <counter>       Print internal counter for <counter> (repeatable)
      --track-untyped [={[nowhere],everywhere,everywhere-but-tests}(=everywhere)]
                                Include a per-file counter of untyped usages in the
                                `--print=file-table-<format>` output. This is in addition
                                to the codebase-wide `types.input.untyped.usages`
                                counter.
      --metrics-file <file>     Report counters and some timers to <file>, in JSON
                                format. (default: "")
      --metrics-prefix <string>
                                String to prefix all metrics with, e.g. `my_org.my_repo`.
                                (default: ruby_typer.unknown)
      --metrics-branch <branch>
                                Branch to report in metrics export. (default: none)
      --metrics-sha <sha>       Set the `sha` field to <sha> in the `--metrics-file`
                                output. (default: none)
      --metrics-repo <repo>     Set the `repo` field to <repo> in the `--metrics-file`
                                output. (default: none)
      --statsd-host <host>      StatsD sever hostname (default: "")
      --statsd-prefix <prefix>  StatsD prefix (default: ruby_typer.unknown)
      --statsd-port <port>      StatsD server port (default: 8200)
      --metrics-extra-tags <key1>=<value1>,<key2>=<value2>
                                Extra tags to include in every statsd metric, comma
                                separated. (default: "")

```

## LSP options

```plaintext
      --lsp                     Start in language server protocol mode (LSP mode)
      --lsp-error-cap <cap>     Reports no more than <cap> diagnostics (e.g. errors and
                                informations) to the language client, like VS Code. Can
                                prevent slowdown triggered by large diagnostic lists. A
                                <cap> of 0 means no limit. (default: 1000)
      --disable-watchman        When in LSP mode, disable file watching via Watchman
      --watchman-path <path>    Path to watchman executable. Will search on `PATH` if
                                <path> contains no slashes. (default: watchman)
      --watchman-pause-state-name <state>
                                Name of watchman state that halts processing for its
                                duration (default: "")
      --lsp-directories-missing-from-client <path>
                                Directory prefixes that only exist where the LSP server
                                is running, not on the client. Useful when running Sorbet
                                via an `ssh` connection to a remote server, where the
                                remote server has generated files that do not exist on
                                the client. References to files in these directories will
                                be sent as `sorbet:` URIs to clients that understand
                                them.

```

## LSP FEATURE options

```plaintext
      --enable-experimental-lsp-document-formatting-rubyfmt
                                Enable experimental LSP feature: Document Formatting with
                                Rubyfmt
      --rubyfmt-path <path>     Path to the rubyfmt executable used for document
                                formatting. Will search on `PATH` if <path> contains no
                                slashes. (default: rubyfmt)
      --enable-experimental-lsp-document-highlight
                                Enable experimental LSP feature: Document Highlight
      --enable-experimental-lsp-signature-help
                                Enable experimental LSP feature: Signature Help
      --enable-experimental-lsp-extract-to-variable
                                Enable experimental LSP feature: Extract To Variable
      --enable-all-experimental-lsp-features
                                Enable every experimental LSP feature. (WARNING: can be
                                crashy; for developer use only. End users should prefer
                                to use `--enable-all-beta-lsp-features`, instead.)
      --enable-all-beta-lsp-features
                                Enable (expected-to-be-non-crashy) early-access LSP
                                features.

```

## PERFORMANCE options

```plaintext
      --web-trace-file <file>   Generate a trace into <file> in the Trace Event Format
                                (used by chrome://tracing) (default: "")
      --cache-dir <dir>         Use <dir> to cache certain data. Will create <dir> if it
                                does not exist (default: "")
      --max-cache-size-bytes <bytes>
                                Must be a multiple of OS page size (usually 4096).
                                Subject to restrictions on mdb_env_set_mapsize function
                                in LMDB API docs. (default: 4294967296)
      --max-threads <n>         Set number of threads to use for fork/join worker pools.
                                LSP will <n> threads plus some extra threads to manage
                                the connection with the client, watchman, etc. (default:
                                number of cores on the system)
      --reserve-class-table-capacity <n>
                                Preallocate <n> slots in the class and modules table
                                (default: 0)
      --reserve-method-table-capacity <n>
                                Preallocate <n> slots in the method table (default: 0)
      --reserve-field-table-capacity <n>
                                Preallocate <n> slots in the field table (default: 0)
      --reserve-type-argument-table-capacity <n>
                                Preallocate <n> slots in the type argument table
                                (default: 0)
      --reserve-type-member-table-capacity <n>
                                Preallocate <n> slots in the type member table (default:
                                0)
      --reserve-utf8-name-table-capacity <n>
                                Preallocate <n> slots in the UTF8 name table (default: 0)
      --reserve-constant-name-table-capacity <n>
                                Preallocate <n> slots in the constant name table
                                (default: 0)
      --reserve-unique-name-table-capacity <n>
                                Preallocate <n> slots in the unique name table (default:
                                0)

```

## STRIPE PACKAGES MODE options

```plaintext
      --stripe-packages         Enable support for Stripe's internal Ruby package system
      --stripe-packages-hint-message arg
                                Optional hint message to add to packaging related errors
                                (default: "")
      --extra-package-files-directory-prefix-underscore string
                                Extra parent directories which contain package files.
                                These paths use an underscore package-munging convention,
                                i.e. 'Project_Foo'
      --extra-package-files-directory-prefix-slash string
                                Extra parent directories which contain package files.
                                These paths use a slash package-munging convention, i.e.
                                'project/foo'
      --allow-relaxed-packager-checks-for string
                                Packages which are allowed to ignore the restrictions set
                                by `visible_to` and `export` directives
      --package-skip-rbi-export-enforcement string
                                Constants defined in RBIs in these directories (and no
                                others) can be exported

```

## STRIPE AUTOGEN options

```plaintext
      --autogen-version arg     Autogen version to output
      --autogen-constant-cache-file arg
                                Location of the cache file used to determine if it's safe
                                to skip autogen. If this is not provided, autogen will
                                always run. (default: "")
      --autogen-changed-files arg
                                List of files which have changed since the last autogen
                                run. If a cache file is also provided, autogen may exit
                                early if it determines that these files could not have
                                affected the output of autogen.
      --autogen-subclasses-parent string
                                Parent classes for which generate a list of subclasses.
                                This option must be used in conjunction with -p
                                autogen-subclasses
      --autogen-subclasses-ignore string
                                Like --ignore, but it only affects `-p
                                autogen-subclasses`.
      --autogen-behavior-allowed-in-rbi-files-paths string
                                RBI files defined in these paths can be considered by
                                autogen as behavior-defining.
      --autogen-msgpack-skip-reference-metadata
                                Skip serializing extra metadata on references when
                                printing msgpack in autogen

```

## DEBUGGING options

```plaintext
  -v, --verbose                 Verbosity level [0-3]
      --debug-log-file <file>   Path to debug log file (default: "")
      --wait-for-dbg            Wait for debugger to attach after starting. Especially
                                useful to attach to a Sorbet process launched by a
                                language client
      --sleep-in-slow-path [=arg(=3)]
                                Add some sleeps to slow path to artificially slow it down
      --stress-incremental-resolver
                                Simulate updates which tend to expose namer and resolver
                                bugs
      --simulate-crash          Raise an uncaught C++ exception on startup to simulate a
                                crash
      --force-hashing           Force Sorbet to calculate file hashes, even from the CLI.
                                Useful for profiling.
      --trace-lexer             Emit the lexer's token stream in a debug format
      --trace-parser            Enable bison's parser trace functionality
      --dump-package-info arg   Dump package info in JSON form to the given file.
                                (default: "")
  -p, --print <format>          Print various internal data structures.
                                By default, the output is to stdout. To send the data to
                                a file, use
                                --print=<format>:<file>
                                Most of these formats are unstable, for internal-use only.

                                Stable: [file-table-json, file-table-proto,
                                file-table-messagepack, file-table-full-json,
                                file-table-full-proto, file-table-full-messagepack,
                                missing-constants, payload-sources, untyped-blame]

                                Unstable: [parse-tree, parse-tree-json,
                                parse-tree-json-with-locs, parse-tree-whitequark,
                                desugar-tree, desugar-tree-raw, rewrite-tree,
                                rewrite-tree-raw, index-tree, index-tree-raw, name-tree,
                                name-tree-raw, resolve-tree, resolve-tree-raw,
                                flatten-tree, flatten-tree-raw, ast, ast-raw, cfg,
                                cfg-raw, cfg-text, symbol-table, symbol-table-raw,
                                symbol-table-json, symbol-table-proto,
                                symbol-table-messagepack, symbol-table-full,
                                symbol-table-full-raw, symbol-table-full-json,
                                symbol-table-full-proto, symbol-table-full-messagepack,
                                autogen, autogen-msgpack, autogen-subclasses,
                                package-tree, minimized-rbi

```

## INTERNAL options

```plaintext
      --suppress-non-critical   Exit 0 unless there was a critical error (i.e., an
                                uncaught exception)
      --no-stdlib               Do not load Sorbet's payload which defines RBI files for
                                the Ruby standard library
      --store-state file        Store state into file (default: "")
      --silence-dev-message     Silence "You are running a development build" message
      --censor-for-snapshot-tests
                                When printing raw location information, don't show line
                                numbers

```

## OTHER options

```plaintext
      --version                 Show Sorbet's version
      --license                 Show Sorbet's license, and licenses of its dependencies
      --stdout-hup-hack         Monitor STDERR for HUP and exit on hangup to work around
                                OpenSSH bug
      --minimize-to-rbi <file.rbi>
                                [experimental] Output a minimal RBI containing the diff
                                between Sorbet's view of a codebase and the definitions
                                present in this file (default: "")
      --single-package arg      Run in single-package mode (default: "")
      --package-rbi-generation  Enable rbi generation for stripe packages
      --package-rbi-dir arg     The location of generated package rbis (default: "")
  -h, --help [=SECTION(=all)]   Show help. Can pass an optional SECTION to show help for
                                only one section instead of the default of all sections

```