// swift-tools-version:3.1

import PackageDescription

let package = Package(
    name: "Exchange",
    targets:
    [
        Target(name: "Exchange", dependencies: ["ExchangeLib"]),
        Target(name: "ExchangeLib")
    ]
)
