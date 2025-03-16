import ctypes

# Load the shared library
solver_lib = ctypes.CDLL(r"./c4solver_c_interface.dll")

# Define argument and return types
solver_lib.solver_init.argtypes = [ctypes.c_char_p]
solver_lib.solver_init.restype = ctypes.POINTER(ctypes.c_void_p)

solver_lib.solver_delete.argtypes = [ctypes.POINTER(ctypes.c_void_p)]
solver_lib.solver_delete.restype = None

solver_lib.solver_solve.argtypes = [ctypes.POINTER(ctypes.c_void_p), ctypes.c_char_p, ctypes.c_bool, ctypes.c_bool, ctypes.c_char_p, ctypes.c_size_t]
solver_lib.solver_solve.restype = ctypes.c_char_p

# Example usage
handle = solver_lib.solver_init(None) #or None for default
result_buffer = ctypes.create_string_buffer(256)
result = solver_lib.solver_solve(handle, b"123", True, False, result_buffer, 256)
print(result.decode())

result = solver_lib.solver_solve(handle, b"123445622", False, True, result_buffer, 256)
print(result.decode())

solver_lib.solver_delete(handle)
