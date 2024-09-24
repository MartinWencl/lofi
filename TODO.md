# TODO List for Lofi

## Plan and Tasks

- [ ] Get to state 0.1 - MVP
    - go
        - [x] Basic fyne window, hide decorations
        - [x] Text input and a list of labels/buttons w/o decotaions?
        - [ ] Setting  a callbacks on select
        - [ ] Prepare for later styling
    - Lua
        - [ ] Integrate the Golua library to interact with LuaJIT.
        - [ ] Set up Lua environment and load the `lofi.lua` config file.
        - [ ] Expose Go functions to Lua for configuring Lofi.
        - [ ] Design an API (v0.1 again MVP) inspired by Neovim for configuring Lofi.
            - TODO: Define API MVP
    - functionality
        - [ ] file picker

### Future steps
- [ ] Implement robust error handling for Lua integration.
- [ ] Add logging for debugging and monitoring purposes.
- [ ] Write unit tests for the Go functions.
- [ ] Write Lua scripts to test the Lua API functions.
- [ ] Create documentation for setting up and configuring Lofi using `lofi.lua`.
- [ ] Provide usage examples and best practices.

### Possible future tasks/features
- [ ] Add support for custom themes and styling.
    - [ ] Dynamic widget creation from lua?

