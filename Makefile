# holochain-rust Makefile
# currently only supports 'debug' builds

# run `make` to build all the libraries and binaries
# run `make test` to execute all the tests
# run `make clean` to clean up the build environment

all: main

# list all the "C" binding tests that have been written
C_BINDING_DIRS = $(sort $(dir $(wildcard c_binding_tests/*/)))

# list all the "C" binding test executables that should be produced
C_BINDING_TESTS = $(foreach dir,$(C_BINDING_DIRS),target/debug/c_binding_tests/$(shell basename $(dir))/test_executable)

# list all the extraneous files that will be generated when running tests
C_BINDING_CLEAN = $(foreach dir,$(C_BINDING_DIRS),$(dir)Makefile $(dir).qmake.stash)

# build artifact / dependency checking is handled by our sub-tools
# we can just try to build everything every time, it should be efficient
.PHONY: main \
	c_binding_tests ${C_BINDING_DIRS} \
	test ${C_BINDING_TESTS} \
	clean ${C_BINDING_CLEAN}

# apply formatting / style guidelines, and build the rust project
main:
	cargo fmt
#	cargo +nightly clippy -- --deny clippy
# TODO: clippy causing prolbems, disabled for now until issues fixed see #27: https://waffle.io/holochain/org/cards/5b297aa42e3a9d002f6d055d
	cargo build --verbose --all --features "strict"

# list all our found "C" binding tests
c_binding_tests: ${C_BINDING_DIRS}

# build all our found "C" binding tests
${C_BINDING_DIRS}:
	qmake -o $@Makefile $@qmake.pro
	cd $@; $(MAKE)

# execute all tests, both rust and "C" bindings
test: main c_binding_tests ${C_BINDING_TESTS}
	rustup target add wasm32-unknown-unknown --toolchain nightly
	cd hc_core/src/nucleus/wasm-test && cargo build --target wasm32-unknown-unknown
	cargo test --verbose --all --features "strict"

# execute all the found "C" binding tests
${C_BINDING_TESTS}:
	$@

# clean up the target directory and all extraneous "C" binding test files
clean: ${C_BINDING_CLEAN}
	-@$(RM) -rf target

# clean up the extraneous "C" binding test files
${C_BINDING_CLEAN}:
	-@$(RM) $@