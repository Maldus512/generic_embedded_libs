
TEST = "test"
LIBS = ["generic/debounce", "generic/keyboard", "generic/parameter", "generic/circularbuffer"]



env_options = {
    "CPPPATH" : LIBS,
    "CFLAGS" : ["-g"]
}


sources = ["test.c"]
for lib in LIBS:
    sources += Glob("{}/*.c".format(lib))

env = Environment(**env_options)

env.Program(TEST, sources)