FROM alpine as build
RUN apk add build-base
WORKDIR /app
COPY . /app
RUN make build
FROM busybox:latest
COPY --from=build /app/decoder .
ENTRYPOINT ["./decoder"]