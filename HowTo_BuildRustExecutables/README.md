# Build RUST Execuables

## Every package is bundled to a specific firmware version: For Version 17 - FW:03.05.10(99)

This HowTo shows how to cross-compile executables written in [RUST](https://www.rust-lang.org/) for PFC's.

## 1) Install PFC Firmware SDK

Follow the instructions on [https://github.com/WAGO/pfc-firmware-sdk](https://github.com/WAGO/pfc-firmware-sdk) to install PFC Firmware SDK.

## 2) Install rustup (https://rustup.rs/)

Follow the instructions on [https://rustup.rs/](https://rustup.rs/) to install `rustup`,
the RUST toolchain manager.

## 3) Add Rust Target

In order to compile executables for PFC's the `armv7-unknown-linux-gnueabihf` target is needed.
Use `rustup` to add this target.

    rustup target add armv7-unknown-linux-gnueabihf

## 4) Setup Cargo

To link execuables, Rust needs to know which linker should be used.  
The linker is part of the toolchain installed along with the PFC firmware SDK.

It is assumed, the toolchain was installed to `/opt/LINARO.Toolchain-2017.10`.

Add the following lines to `~/.cargo/config` file.  
_(Create this file, if it does not exists.)_

````
[build]
target = "armv7-unknown-linux-gnueabihf"

[target.armv7-unknown-linux-gnueabihf]
linker = "/opt/LINARO.Toolchain-2017.10/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc"
````

## 5) Create a Rust Project

Create a directory and use `cargo` to create a Hello-World project.

````
mkdir hello
cd hello
cargo init
````

Compile the project using `cargo`.

    cargo build --target armv7-unknown-linux-gnueabihf

The resulting binary can be found at `target/armv7-unknown-linux-gnueabihf/release/hello`.

## 6) Create stripped Release Build (Optional)

The resulting binary of step 5) is a little large.

````
ls -lah target/armv7-unknown-linux-gnueabihf/debug/hello
-rwxrwxr-x 2 user user 3,1M Nov 10 00:03 target/armv7-unknown-linux-gnueabihf/debug/hello
````

For release purposes, it should be build optimized and stripped.

### 6.1) Add Release Profile to Cargo.toml

Add the following lines to `Cargo.toml`.

````
[profile.release]
opt-level = "z"
lto = true
````

These lines activate optimization for size (`opt-level = "z"`) and link time
optimization (`lto = true`).

## 6.2) Compile Release

To compile release binary, use `cargo`.

    cargo build --target armv7-unknown-linux-gnueabihf --release

The resulting binary can be found at `target/armv7-unknown-linux-gnueabihf/release/hello`.

## 6.3) Strip Binary

Finally, the binary should be stripped to ge rid of any unnecessary symbols.
Therefore, the tool `strip` of PFC's toolchain is used.

    /opt/LINARO.Toolchain-2017.10/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-strip target/armv7-unknown-linux-gnueabihf/release/hello

The size of resulting binary should now be much smaller.

````
ls -lah target/armv7-unknown-linux-gnueabihf/release/hello
-rwxrwxr-x 2 user user 226K Nov 10 00:36 target/armv7-unknown-linux-gnueabihf/release/hello
````
