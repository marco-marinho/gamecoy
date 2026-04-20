from cffi import FFI

if __name__ == "__main__":
    ffibuilder = FFI()

    ffibuilder.set_source("gamecoy", """
        #include "cpu.h"
        """, libraries=["GameCoy"], library_dirs=["."], include_dirs=["../include"]
    )

    with open("../include/instructions.h") as f:
        lines = f.readlines()
        valid_instruction_lines = [line for line in lines if not line.startswith("#")]

    with open("../include/cpu.h") as f:
        lines = f.readlines()
        valid_cpu_lines = [line for line in lines if not line.startswith("#")]

    definitions = "\n".join(valid_instruction_lines + valid_cpu_lines)


    ffibuilder.cdef(definitions)
    ffibuilder.compile(verbose=True)