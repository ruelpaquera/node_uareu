{
    "targets": [{
        "target_name": "uareuprint",
		"sources": [ "src/fingerprint.cpp" , "src/scan.cpp" , "src/selection.cpp" ],
        "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "<!(node -e \"require('zlib')\")", 
            "-L/usr/lib", 
            "Include",
            "-LInclude/dpfpdd.h",
            "-LInclude/dpfj.h",
            "-LInclude/dpfj_quality.h",
            "-LInclude/dpfj_compression.h" 
        ],
        "libraries": [ 
            "-LInclude/dpfpdd.h",
            "-LInclude/dpfj.h",
            "-LInclude/dpfj_quality.h",
            "-LInclude/dpfj_compression.h"
        ],
        "variables": {
            "node_version": '<!(node --version | sed -e "s/^v\([0-9]*\\.[0-9]*\).*$/\\1/")',
        },
        "target_conditions": [
            [ "node_version == '0.10'", { "defines": ["OLD_UV_RUN_SIGNATURE"] } ]
        ]
    }]
}