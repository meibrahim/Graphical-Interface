// import express (after npm install express)
const express = require('express');
const bodyParser = require('body-parser')
const path = require('path')
const fs = require('fs');	
// create new express app and save it as "app"
const app = express();

// server configuration
const PORT = 8080;

let data;

app.use((req, res, next) => {
	res.header('Access-Control-Allow-Origin', '*');
	res.header('Access-Control-Allow-Headers',
      'Origin, X-Requested-With, Content-Type, Accept');
  res.header("Access-Control-Allow-Methods", "GET,HEAD,OPTIONS,POST,PUT");
	next();
})
app.use(bodyParser.json())

app.use(express.static(path.join(__dirname, '../build')))

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, '../build', 'index.html'))
})

const readData = () => {
	try {
		data = JSON.parse(fs.readFileSync('server/data.json'))
		console.log("Succesfully readed data")
	} catch (err) {
		console.log(err)
	}
}

const saveData = () => {
  fs.writeFileSync('server/data.json', JSON.stringify(data))
}

readData();

// create a route for the app
app.get('/data', (req, res) => {
  res.send(data);
});

app.post('/save', (req, res) => {
	const reqData = req.body;
	foundActivity = data.activities.find(activity => {
		return reqData.slot == activity.slot &&
				reqData.room == activity.room
	})
	if (foundActivity) {
		const indexOfFound = data.activities.indexOf(foundActivity);
		data.activities.splice(indexOfFound, 1, reqData)
		saveData();
		readData();
		res.send('Modified record')
	}
	else {
		data.activities.push(reqData);
		saveData();
		readData();
		res.send('Added record')
	}
});

app.post('/delete', (req, res) => {
  const reqData = req.body;
	foundActivity = data.activities.find(activity => {
		return reqData.slot === activity.slot &&
				reqData.room === activity.room
	})
	if (foundActivity) {
		const foundIndex = data.activities.indexOf(foundActivity);
		data.activities.splice(foundIndex, 1);
		saveData();
		readData();
		res.send("Record successfully deleted")
	}
	else {
		res.status(500).send('There is no such record')
	}
});


// make the server listen to requests
app.listen(PORT, () => {
  console.log(`Server running at: http://localhost:${PORT}/`);
});