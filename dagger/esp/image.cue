package esp

import "universe.dagger.io/docker"

#Image: docker.#Build & {
    tag: string | *"v5.0"
    steps: [
        docker.#Pull & {
            source: "index.docker.io/espressif/idf:\(tag)"
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