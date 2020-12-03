## @file utils.py
# Util functions.
#
# Defines a set of util frequently used functions.
#
# @author Ricardo Dutra da Silva

import os
import sys
import math
import numpy as np
import OpenGL.GL as gl

## Read shader.
#
# Read a shader from a file.
#
# @param File name.
# @return String with shader code.
def readShaderFile(shader_file):

    # Check if file exists.
    if not os.path.isfile(shader_file):
        print("Could not open shader file: " + shader_file)
        sys.exit()

    # Read file.
    shader_code = None
    with open(shader_file, 'r') as f:
        shader_code = f.read()

    return shader_code


## Create program.
#
# Creates a program from given shader codes.
#
# @param vertex_code String with code for vertex shader.
# @param fragment_code String with code for fragment shader.
# @return Compiled program.
def createShaderProgram(vertex_code, fragment_code):
    
    # Request a program and shader slots from GPU
    program  = gl.glCreateProgram()
    vertex   = gl.glCreateShader(gl.GL_VERTEX_SHADER)
    fragment = gl.glCreateShader(gl.GL_FRAGMENT_SHADER)
    
    # Set shaders source
    gl.glShaderSource(vertex, vertex_code)
    gl.glShaderSource(fragment, fragment_code)

    # Compile shaders
    gl.glCompileShader(vertex)
    if not gl.glGetShaderiv(vertex, gl.GL_COMPILE_STATUS):
        error = gl.glGetShaderInfoLog(vertex).decode()
        print(error)
        raise RuntimeError("Shader compilation error")
                
    gl.glCompileShader(fragment)
    if not gl.glGetShaderiv(fragment, gl.GL_COMPILE_STATUS):
        error = gl.glGetShaderInfoLog(fragment).decode()
        print(error)
        raise RuntimeError("Shader compilation error")                

    # Attach shader objects to the program
    gl.glAttachShader(program, vertex)
    gl.glAttachShader(program, fragment)

    # Build program
    gl.glLinkProgram(program)
    if not gl.glGetProgramiv(program, gl.GL_LINK_STATUS):
        print(gl.glGetProgramInfoLog(program))
        raise RuntimeError('Linking error')

    # Get rid of shaders (not needed anymore)
    gl.glDetachShader(program, vertex)
    gl.glDetachShader(program, fragment)
    gl.glDeleteShader(vertex)
    gl.glDeleteShader(fragment)

    return program
