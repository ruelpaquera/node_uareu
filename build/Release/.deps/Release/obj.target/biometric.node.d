cmd_Release/obj.target/biometric.node := g++ -shared -pthread -rdynamic -m64  -Wl,-soname=biometric.node -o Release/obj.target/biometric.node -Wl,--start-group Release/obj.target/biometric/src/new/verify.o Release/obj.target/biometric/src/new/base64.o Release/obj.target/biometric/src/new/main.o Release/obj.target/biometric/src/new/capture.o Release/obj.target/biometric/src/new/selection.o Release/obj.target/biometric/src/new/helpers.o -Wl,--end-group -L/home/ruel/node/node_uareu/Include -L/home/ruel/node/node_uareu/lib /usr/lib/libdpfpdd.so /usr/lib/libdpfj.so /usr/lib/libtfm.so /home/ruel/node/node_uareu/lib/libWSQ_library64.so
