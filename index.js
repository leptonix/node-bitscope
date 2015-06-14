var async = require('async')

var bindings = require('./build/Release/bitscope')

function main(options, callback) {
  if ((typeof options) == 'function') {
    callback = options
    options = null
  }
  if (!options) {options = {}}
  var device = options.device || 0
  var rate = options.rate || 100000
  var size = options.size || 1000
  var channel = options.channel || 0
  if ((typeof channel) == 'string') {
    channel = channel.split(',').map(function(a){return parseInt(a)})
  }
  if (!Array.isArray(channel)) {channel = [channel]}
  bindings.init(parseInt(device), function(error){
    if (error) {return callback(error)}
    async.map(channel, bindings.setup, function(error){
      if (error) {return callback(error)}
      bindings.trace(rate, size, function(error, data){
        if (error) {return callback(error)}
        async.map(
          channel,
          function(item, callback){
            bindings.acquire(item, size, callback)
          },
          function(error, data){
            if (error) {return callback(error)}
            callback(
              null,
              channel.map(function(c, i){
                return {
                  channel: c,
                  trace: data[i]
                }
              })
            )
            bindings.close(function(){})
          }
        )
      })
    })
  })
}

['init', 'setup', 'trace', 'acquire', 'close'].forEach(function(name){
  main[name] = bindings[name]
})

module.exports = main
