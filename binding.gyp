{
  "targets": [
    {
      "target_name": "bitscope",
      "sources": [
        "src/bitscope.cc",
        "src/init.cc",
        "src/setup.cc",
        "src/acquire.cc",
        "src/trace.cc",
        "src/close.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"],
      "libraries": [
        "-lBitLib"
      ]
    }
  ]
}
