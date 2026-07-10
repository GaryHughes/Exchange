import Foundation
import ExchangeLib

// readLine() and String.components(separatedBy:) are well known bottlenecks for
// high-volume stdin processing in Swift - Unicode-aware line assembly and
// grapheme-cluster-aware splitting are real, avoidable costs at 10M-line scale.
// Read stdin in large raw chunks instead and split on the ':'/'\n' bytes directly.

let exchange = Exchange()
let newline: UInt8 = 0x0A
let carriageReturn: UInt8 = 0x0D
let chunkSize = 1 << 20 // 1 MB

func processLine(_ line: Data) {
    var line = line
    if let last = line.last, last == carriageReturn {
        line.removeLast()
    }
    do {
        if let order = try Order(fromBytes: line) {
            for trade in exchange.insert(order: order) {
                print(trade.toString())
            }
        }
    } catch let error {
        print("Error while processing orders: \(error)")
    }
}

let input = FileHandle.standardInput
var leftover = Data()

while true {
    let chunk = input.readData(ofLength: chunkSize)
    if chunk.isEmpty {
        break
    }

    var buffer = leftover
    buffer.append(chunk)

    var start = buffer.startIndex
    while let newlineIndex = buffer[start...].firstIndex(of: newline) {
        processLine(buffer[start..<newlineIndex])
        start = buffer.index(after: newlineIndex)
    }

    leftover = Data(buffer[start...])
}

if !leftover.isEmpty {
    processLine(leftover)
}
