PROTOC = $(NANOPB)/generator-bin/protoc --proto_path=src/api --nanopb_out="-v:src"

.PHONY: proto

proto:
	$(PROTOC) src/api/*.proto
