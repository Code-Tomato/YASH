#!/bin/bash

# Script to generate compile_commands.json for clangd
# This runs a clean build without running tests to avoid segfaults

echo "Generating compile_commands.json for clangd..."

# Clean first
make clean

# Use bear to capture compilation commands during build
bear -- make all

echo "compile_commands.json generated successfully!"
echo "VSCode clangd should now work properly with Unity headers."
