const express = require('express');
const router = express.Router();
const MbusData = require('../models/mbusData');

// Route to get all data
router.get('/', async (req, res) => {
    try {
        const data = await MbusData.find();
        res.json(data);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
});

// Route to add new data
router.post('/', async (req, res) => {
    const data = new MbusData({
        value: req.body.value
    });

    try {
        const newData = await data.save();
        res.status(201).json(newData);
    } catch (err) {
        res.status(400).json({ message: err.message });
    }
});

module.exports = router;
