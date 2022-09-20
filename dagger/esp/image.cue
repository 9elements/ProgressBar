package esp

import "universe.dagger.io/docker"

#Image: docker.#Build & {
    tag: string | *"v4.4.2"
    steps: [
        docker.#Pull & {
            source: "index.docker.io/espressif/idf:\(tag)"
        },
        docker.#Run & {
            command: {
                name: "apt"
                args: ["update"]
            }
        },
        docker.#Run & {
            command: {
                name: "apt"
                args: ["install", "clang", "llvm"]
                flags: {
                    "-y": true
                }
            }
        },
        docker.#Run & {
            command: {
                name: "curl"
                args: ["https://sh.rustup.rs"]
                flags: {
                    "-o": "/tmp/rustup.sh"
                    "--proto": "https"
                    "--tlsv1.2": true
                    "-f": true
                    "-s": true
                    "-S": true
                }
            }
        },
        docker.#Run & {
            command: {
                name: "sh"
                args: ["/tmp/rustup.sh", "-v", "-y", "--default-toolchain", "nightly", "-c", "rust-src", "-t", "riscv32i-unknown-none-elf"]
            }
            env: {
                "RUSTUP_HOME": "/usr"
                "CARGO_HOME": "/usr"
            }
        },
        docker.#Run & {
            command: {
                name: "curl"
                args: ["https://taskfile.dev/install.sh"]
                flags: {
                    "-o": "/tmp/taskfile.sh"
                    "--proto": "https"
                    "--tlsv1.2": true
                    "-f": true
                    "-s": true
                    "-S": true
                }
            }
        },
        docker.#Run & {
            command: {
                name: "sh"
                args: ["/tmp/taskfile.sh", "-d", "-b", "/usr/bin"]
            }
        },
    ]
}