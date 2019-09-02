{
    "targets": [{
        "target_name": "biometric",
		"sources": [ "src/verify.cpp", "src/wsq_to_bmp.cpp", "src/base64.cpp", "src/main.cpp"  , "src/selection.cpp" , "src/helpers.cpp" , "src/enrollment.cpp" , "src/capture.cpp" ],
        "conditions": [
            ["OS in \"linux\"",{                
                "include_dirs": [
                    "<!(node -e \"require('nan')\")",
                    "<!(node -e \"require('zlib')\")", 
                    "/opt/Crossmatch/urusdk-linux/Include"
                ],
                "libraries": [
                "<!(node -e \"require('zlib')\")",  
                    "/usr/lib/libdpfpdd.so",
                    "/usr/lib/libdpfj.so",
                    "/usr/lib/libtfm.so",
                    "./../lib/libWSQ_library64.so"
                ]
            }], 
            ["OS in \"win\"",{                
                "include_dirs": [
                    "<!(node -e \"require('nan')\")",
                    "<!(node -e \"require('zlib')\")", 
                    "./Include",
                    "./lib"
                ],         
                "libraries": [
                    "<!(node -e \"require('zlib')\")",  
                    "-l/Include",
                    "-l/lib"
                ]
            }]
        ],
        "variables": {
            "node_version": '<!(node --version | sed -e "s/^v\([0-9]*\\.[0-9]*\).*$/\\1/")',
        },
        "target_conditions": [
            [ "node_version == '0.10'", { "defines": ["OLD_UV_RUN_SIGNATURE"] } ]
        ]
    }]
}