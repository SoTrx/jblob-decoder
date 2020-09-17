# Basic JBlob decoder

This programs decodes a "jblob" files, which is a set of JPEGs files concatenated together.

## Usage

    decoder <jblob_file>

### Using Docker
You can either use the DockerHub image or build it yourself.

#### Pulling it from the DockerHub
    docker pull dockerutils/decoder

#### Building the image
    docker build -t decoder .

The Docker image can be used by sharing the directory the .jblob are in.

```sh
# Folder structure
pwd
└───data
# Command
docker run -v $PWD/data:/data -it dockerutils/decoder:latest data/example.jblob
````
