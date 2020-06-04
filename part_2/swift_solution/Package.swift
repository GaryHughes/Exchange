// swift-tools-version:5.1

import PackageDescription

let package = Package(
    name: "Exchange",
    targets: [
        .target(name: "Exchange", dependencies: ["ExchangeLib"]),
        .target(name: "ExchangeLib"),
        .testTarget(name: "Tests", dependencies: ["ExchangeLib"], path: "Tests")
    ]
)
