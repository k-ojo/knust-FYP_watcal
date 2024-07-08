const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const path = require('path');

const app = express();
const port = 3000;

// Middleware
app.use(bodyParser.json());
app.use(express.static('public'));
app.set('view engine', 'ejs');

// MongoDB connection
mongoose.connect('mongodb://localhost:27017/mbus', { useNewUrlParser: true, useUnifiedTopology: true })
    .then(() => console.log('MongoDB connected'))
    .catch(err => console.log(err));

// Define a simple route
app.get('/', (req, res) => {
    res.render('index');
});

// Route to read from M-Bus and save to database
app.get('/read', (req, res) => {
    exec('./master', (error, stdout, stderr) => {
        if (error) {
            console.error(`exec error: ${error}`);
            return res.status(500).json({ message: 'Error reading M-Bus' });
        }
        console.log(`stdout: ${stdout}`);
        console.error(`stderr: ${stderr}`);

        // Assuming the master program outputs the value
        const value = parseFloat(stdout);
        const data = new MbusData({ value });

        data.save()
            .then(() => res.redirect('/data'))
            .catch(err => res.status(400).json({ message: err.message }));
    });
});

// Route to display data
app.get('/data', async (req, res) => {
    try {
        const data = await MbusData.find().sort({ timestamp: -1 });
        res.render('data', { data });
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
});

app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
});
