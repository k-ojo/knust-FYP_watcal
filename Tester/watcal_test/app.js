const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');
const { exec } = require('child_process');

const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use(express.static('public'));

// Connect to MongoDB
mongoose.connect('mongodb://localhost:27017/mbus', { useNewUrlParser: true, useUnifiedTopology: true });
const db = mongoose.connection;

db.on('error', console.error.bind(console, 'connection error:'));
db.once('open', () => {
  console.log('Connected to MongoDB');
});

// Define a schema and model for meter data
const meterSchema = new mongoose.Schema({
  address: String,
  data: String,
  timestamp: { type: Date, default: Date.now }
});

const Meter = mongoose.model('Meter', meterSchema);

// Route to fetch data from the meters
app.get('/scan', (req, res) => {
  exec('mbus-serial-scan -d /dev/pts/1', (error, stdout, stderr) => {
    if (error) {
      console.error(`exec error: ${error}`);
      return res.status(500).json({ error: error.message });
    }
    // Process and store the data in MongoDB
    const dataArray = stdout.split('\n').filter(line => line.includes('RECV'));
    dataArray.forEach(data => {
      const [timestamp, address, rawData] = data.split(' ').slice(0, 3);
      const newMeterData = new Meter({ address, data: rawData });
      newMeterData.save((err) => {
        if (err) return console.error(err);
      });
    });
    res.json({ message: 'Data scanned and stored', data: dataArray });
  });
});

// Route to get meter data from MongoDB
app.get('/meters', (req, res) => {
  Meter.find({}, (err, meters) => {
    if (err) return res.status(500).json({ error: err.message });
    res.json(meters);
  });
});

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});
