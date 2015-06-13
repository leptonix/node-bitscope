{
  "targets": [
    {
      "target_name": "bitscope",
      "sources": [
        "bitscope.cc",
        "capture.cc",
        "test.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"],
      "libraries": [
        "-lBitLib"
      ]
    }
  ]
}
