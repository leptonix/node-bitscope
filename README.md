bitscope
========

BitScope library bindings for [node.js](http://nodejs.org/)

```javascript
var bitscope = require('bitscope')

bitscope(
  {
    device: 0,
    channel: [0, 1],
    rate: 100000,
    size: 2000
  },
  function(error, data) {
    if (!error) {
      console.log(data)
    } else {
      console.error(error)
    }
  }
)
```
