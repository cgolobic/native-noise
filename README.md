# native-noise
A demonstration of using NodeJS native modules, with a focus on using native modules in Electron applications.

## Building
NOTE: Windows users should ensure that they have installed [windows-build-tools](https://github.com/felixrieseberg/windows-build-tools)

`npm run build`

For debug builds:

`npm run build:debug`

## Debugging (Visual Studio Code)
### MacOS
#### Electron
TODO

### Windows
#### Electron
1) Start the example Electron app:
```
cd examples/electron
npm start
```
2) In VS Code, select the `(Windows) Attach` debug configuration.
3) Select the `electron` process started in step 1 from the list of processes.
