# Packager

The packeger job is to generate a lookup.c file for the assets, and package them into smaller files in compile time.

## Using the packager tool

```powershell
# Basic packaging
seal-pkg --lookup <lookup-file-name>.c <asset-directory>

# Encrypt packages
seal-pkg -e --lookup <lookup-file-name>.c <asset-directory>
```