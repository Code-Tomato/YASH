# CodeQL Configuration for YASH

This directory contains the CodeQL configuration for the YASH project.

## Files

- `codeql-config.yml` - Main CodeQL configuration file
- `README.md` - This documentation file

## Configuration Details

The CodeQL configuration is set up to:

1. **Scan C/C++ code** using the `cpp` language pack
2. **Use security and quality queries** including:
   - Default security queries
   - Extended security queries
   - Quality queries
3. **Include specific paths**:
   - `src/` - Main source code
   - `include/` - Header files
   - `tests/test_src/` - Test source files
4. **Exclude unnecessary paths**:
   - `docs/` - Documentation
   - `build/` - Build artifacts
   - `tests/unity/` - Unity test framework
   - Various non-source files

## Build Process

The CodeQL analysis uses a manual build process that:

1. Cleans the build directory
2. Generates `compile_commands.json` using `bear`
3. Builds the project with `make all`
4. Runs tests with `make test`

This ensures that CodeQL has complete information about the build process and can analyze all source files properly.

## Workflow

The CodeQL analysis runs:

- On every push to `main` and `develop` branches
- On every pull request to `main` and `develop` branches
- Daily at 2 AM UTC via scheduled runs

## Troubleshooting

If you encounter issues with CodeQL:

1. **Limited file scanning**: Ensure `compile_commands.json` is generated and up-to-date
2. **Permission errors**: Check that the workflow has the required permissions:
   - `actions: read`
   - `contents: read`
   - `security-events: write`
3. **Build failures**: Verify that the build process works locally with `make all`

## Resources

- [CodeQL Documentation](https://codeql.github.com/docs/)
- [GitHub CodeQL Action](https://github.com/github/codeql-action)
- [CodeQL Query Suites](https://codeql.github.com/docs/codeql-cli/creating-codeql-query-suites/)
