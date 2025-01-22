# Stellarium Web Makefile
.PHONY: setup build dev start lint clean gen-es6 gen-es6-debug gen-es6-prof update-engine update-engine-debug

# User ID and Group ID for correct file permissions
USER_UID := $(shell id -u)
USER_GID := $(shell id -g)

# Automatically detect the user's home directory
HOME_DIR := $(shell echo $$HOME)
PROJECT_DIR := $(HOME_DIR)/QUARCS_stellarium-web-engine
FRONTEND_DIR := $(PROJECT_DIR)/apps/web-frontend
BUILD_DIR := $(PROJECT_DIR)/build
ASSETS_DIR := $(FRONTEND_DIR)/src/assets/js
SKYDATA_DIR := $(PROJECT_DIR)/test-skydata

# Docker volume mounts
DOCKER_MOUNTS := -v "$(PROJECT_DIR):/app" -v "$(SKYDATA_DIR):/skydata" $(EXTRA_DOCKER_MOUNTS)

# Generate optimized ES6 JavaScript
gen-es6:
	docker run --rm -it --platform=linux/arm64 \
		-p 8000:8000 -v "$(PROJECT_DIR):/app" swe-dev \
		/bin/bash -c "source /emsdk/emsdk_env.sh && make js-es6"

# Generate debug ES6 JavaScript
gen-es6-debug:
	docker run --rm -it --platform=linux/arm64 \
		-p 8000:8000 -v "$(PROJECT_DIR):/app" swe-dev \
		/bin/bash -c "source /emsdk/emsdk_env.sh && make js-es6-debug"

# Generate profiled ES6 JavaScript
gen-es6-prof:
	docker run --rm -it --platform=linux/arm64 \
		-p 8000:8000 -v "$(PROJECT_DIR):/app" swe-dev \
		/bin/bash -c "source /emsdk/emsdk_env.sh && make js-es6-prof"

# Update the engine with optimized build
update-engine:
	make gen-es6
	cp $(BUILD_DIR)/stellarium-web-engine.js $(ASSETS_DIR)/
	cp $(BUILD_DIR)/stellarium-web-engine.wasm $(ASSETS_DIR)/
	# Ensure sw_helpers.js remains intact
	cp $(FRONTEND_DIR)/src/assets/sw_helpers.js $(ASSETS_DIR)/

# Update the engine with debug build
update-engine-debug:
	make gen-es6-debug
	cp $(BUILD_DIR)/stellarium-web-engine.js $(ASSETS_DIR)/
	cp $(BUILD_DIR)/stellarium-web-engine.wasm $(ASSETS_DIR)/
	# Ensure sw_helpers.js remains intact
	cp $(FRONTEND_DIR)/src/assets/sw_helpers.js $(ASSETS_DIR)/

# Setup: build Docker images and install dependencies
setup: Dockerfile Dockerfile.jsbuild
	# Build Docker image for Emscripten (JS build)
	docker build --platform=linux/arm64 -f Dockerfile.jsbuild -t swe-dev .
	make update-engine
	# Build Docker image for Node.js development
	docker build --platform=linux/arm64 -t stellarium-web-dev \
		--build-arg USER_UID=${USER_UID} --build-arg USER_GID=${USER_GID} .
	# Install dependencies
	docker run --rm -it $(DOCKER_MOUNTS) stellarium-web-dev yarn install

# Run the development server
dev:
	docker run --rm -it -p 8080:8080 -p 8888:8888 $(DOCKER_MOUNTS) stellarium-web-dev yarn run dev

# Run the linter
lint:
	docker run --rm -it $(DOCKER_MOUNTS) stellarium-web-dev yarn run lint

# Build the project for production
build:
	docker run --rm -it $(DOCKER_MOUNTS) stellarium-web-dev yarn run build

# Serve the production build
start:
	cd $(FRONTEND_DIR)/dist && python3 -m http.server 8080

# Update translations
i18n:
	python3 $(FRONTEND_DIR)/tools/update-i18n-en.py

# Clean up unused Docker resources
clean:
	# Remove dangling images, containers, volumes, and networks
	docker system prune -af
	# Optionally, remove specific images
	docker rmi -f swe-dev stellarium-web-dev || true