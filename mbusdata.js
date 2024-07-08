const mongoose = require('mongoose');

const MbusDataSchema = new mongoose.Schema({
    timestamp: { type: Date, default: Date.now },
    value: Number
});

module.exports = mongoose.model('MbusData', MbusDataSchema);
