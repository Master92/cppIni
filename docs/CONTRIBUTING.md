# Welcome to the Contributing Guide for the cppIni project!

We are very happy that you are interested in contributing to cppIni.

This document will provide you with all the information you need to get started.

## Issues

If you find a bug, please open an issue on the GitHub issue tracker. Please make sure that there is no existing issue
for your problem/feature request. If there is an existing issue, please add a comment to the existing issue instead of
creating a new one.
If you are providing a bug report, please make sure to include the following information:

- The version of cppIni you are using
- The compiler you are using
- The operating system you are using
- A minimal example that reproduces the bug
- A description of the expected behavior and the actual behavior

Feature requests are no bug reports, therefore please don't open an issue for a feature request. Instead, please open a
pull request with your feature. Feature requests that are opened as issues will be closed without further notice.

## Pull Requests

If you want to contribute code to cppIni, please open a pull request on GitHub. Please make sure that there is no
existing pull request for your changes. If there is an existing pull request, please add a comment to the existing pull
request instead of creating a new one. Pull requests have to be reviewed by at least one maintainer before they can be
merged. On top of that, all CI builds have to pass.

Please make sure that your pull request follows the following guidelines:

- All pull request have to be based on the `develop` branch
- The pull request has a meaningful title and description
- The pull request is linked to an issue (if applicable)
- The code is documented according to the [Doxygen Style Guide](https://www.doxygen.nl/manual/docblocks.html)
- The code is tested
- There is no merge commit from develop to the pull request's branch
    - If you want to update your branch, please rebase it on the current develop branch prior to opening the pull
      request
    - If there is a merge conflict due to another pull request being merged in the meantime, merge commits are permitted

If your code is not ready to be reviewed yet, please open it as a draft pull request.

## Code of Conduct

Please note that this project is released with a Contributor Covenant Code of Conduct. By participating in this project
you agree to abide by its terms. See [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) for more information.

## License

By contributing to cppIni, you agree that your contributions will be licensed under its GPLv3 license.
See [LICENSE](LICENSE) for more information.
