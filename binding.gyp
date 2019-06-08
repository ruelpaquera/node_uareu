{
    "targets": [{
        "target_name": "uareuprint",
		"sources": [ "src/main.cpp" , "src/identify.cpp" , "src/selection.cpp" , "src/helpers.cpp" ],
        "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "<!(node -e \"require('zlib')\")", 
            "-IInclude",
            "./lib",
            "/opt/Crossmatch/urusdk-linux/Include"
        ],
        "libraries": [
            "<!(node -e \"require('zlib')\")", 
            "-L/opt/Crossmatch/urusdk-linux/Linux/lib", 
            "-L/Include/dpfpdd.h",
            "-L/lib/libdpfpdd.so",
            "-L/lib/libdpfpdd.so",
            "-L/lib/libdpfj.so",
        ],
        "variables": {
            "node_version": '<!(node --version | sed -e "s/^v\([0-9]*\\.[0-9]*\).*$/\\1/")',
        },
        "target_conditions": [
            [ "node_version == '0.10'", { "defines": ["OLD_UV_RUN_SIGNATURE"] } ]
        ]
    }]
}