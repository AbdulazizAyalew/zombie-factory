

FROM alpine:3.19

RUN apk add --no-cache build-base procps
WORKDIR /app
COPY zombie.c .
RUN gcc -o zombie zombie.c

CMD ["./zombie"]