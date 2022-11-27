package main

import (
	"dagger.io/dagger"
	"dagger.io/dagger/core"

	"universe.dagger.io/docker"

	"github.com/9elements/ProgressBar/dagger/esp"
)

dagger.#Plan & {
	client: filesystem: build: write: contents: actions.build.output
	actions: {
		_source: core.#Source & {
			path: "."
			exclude: [
				"hardware",
			]
		}
		build: {
			_image: esp.#Image
			_copy: docker.#Copy & {
				input: _image.output
				contents: _source.output
				dest: "/src"
			}
			_compile: docker.#Run & {
				input: _copy.output
				command: {
					name: "task"
					args: ["build"]
				}
				workdir: "/src"
				export: directories: build: _
			}
			output: _compile.export.directories.build
		}
	}
}
