$(document).ready( function($) {

	var app = {

		selectors: {
			video: $('#videoel'),
			overlay: $('#overlay'),
			enableFaceOverlay: $('#enable-face-overlay'),
			fps: $('#fps'),
			emotions: {
				happy: $('.happy'),
				sad: $('.sad')
			}
		},

		previousDate: Date.now(),

		emotions: {
			happy: 0,
			sad: 0
		},

		displayFaceOverlay: false,
		clientId: window.location.hash.substr(1)

	}

	window.bigDataShowApp = app;
	
	app.selectors.video.attr('width',window.innerWidth-20);

	var vid = document.getElementById('videoel');
	var vid_width = vid.width;
	var vid_height = vid.height;
	var overlay = document.getElementById('overlay');
	var overlayCC = overlay.getContext('2d');

	/*********** Setup of video/webcam and checking for webGL support *********/

	function dist(x0,y0,x1,y1) {
		return Math.sqrt(Math.pow(x1-x0,2)+Math.pow(y1-y0,2));
	}

	function line(x0,y0,x1,y1) {
		overlayCC.beginPath();
		overlayCC.moveTo(x0,y0);
		overlayCC.lineTo(x1,y1);
		
		overlayCC.stroke();
	}

	function rect(x0,y0,w,h) {
		overlayCC.rect(x0-(w/2),y0-(w/2),w,h);
		overlayCC.stroke();
	}

	function enablestart() {
		var startbutton = document.getElementById('start');
		startbutton.value = "startozz";
		startbutton.disabled = null;
	}

	

	function adjustVideoProportions() {
		// resize overlay and video if proportions of video are not 4:3
		// keep same height, just change width
		var proportion = vid.videoWidth/vid.videoHeight;
		vid_height = Math.round(vid_width / proportion);
		vid.height = vid_height;
		overlay.width = vid_width;
		overlay.height = vid_height;
	}

	function gumSuccess( stream ) {
		// add camera stream if getUserMedia succeeded
		if ("srcObject" in vid) {
			vid.srcObject = stream;
		} else {
			vid.src = (window.URL && window.URL.createObjectURL(stream));
		}
		vid.onloadedmetadata = function() {
			adjustVideoProportions();
	
			vid.play();
		}
		vid.onresize = function() {
			adjustVideoProportions();
			if (trackingStarted) {
				ctrack.stop();
				ctrack.reset();
				ctrack.start(vid);
			}
		}
	}

	function gumFail() {
		// fall back to video if getUserMedia failed
		
		document.getElementById('gum').className = "hide";
		document.getElementById('nogum').className = "nohide";
		alert("There was some problem trying to fetch video from your webcam, using a fallback video instead.");
	}

	navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;
	window.URL = window.URL || window.webkitURL || window.msURL || window.mozURL;

	// set up video
	if (navigator.mediaDevices) {
		navigator.mediaDevices.getUserMedia({video : true}).then(gumSuccess).catch(gumFail);
	} else if (navigator.getUserMedia) {
		navigator.getUserMedia({video : true}, gumSuccess, gumFail);
	} else {
		
		document.getElementById('gum').className = "hide";
		document.getElementById('nogum').className = "nohide";
		alert("Your browser does not seem to support getUserMedia, using a fallback video instead.");
	}

	vid.addEventListener('canplay', enablestart, false);

	/*********** Code for face tracking *********/

	var ctrack = new clm.tracker();
	ctrack.init();
	var trackingStarted = false;

	function startVideo() {
		// start video
		vid.play();
		// start tracking
		ctrack.start(vid);
		trackingStarted = true;
		// start loop to draw face
		drawLoop();

		return false;
	}

	function drawLoop() {
		requestAnimFrame(drawLoop);
		overlayCC.clearRect(0, 0, vid_width, vid_height);

		if(app.displayFaceOverlay) {
			if(positions = ctrack.getCurrentPosition()) {
				app.drawPositions(positions)
			}
		}
	
		if(cp = ctrack.getCurrentParameters()) {
			var er = app.ec.meanPredict(cp);

			if(er) {
				app.emotions.happy = er[1].value * 100;
				app.emotions.sad = er[0].value * 100;	
				app.updateGraph();
			
			}
			

			//console.log(er);

		}

		now = Date.now();

		app.elapsed = now-app.previousDate;
		app.previousDate = now;

		
	}

	app.updateGraph = function() {

		app.selectors.emotions.happy.css('width',app.emotions.happy+'%');
		app.selectors.emotions.sad.css('width',app.emotions.sad+'%');

	}

	app.sendData = function() {

		console.log(app.emotions);

		$.ajax({
			type: "POST",
			url: "https://192.168.4.1:1880/data",
			data: {happy: app.emotions.happy,
				   sad:	app.emotions.sad,
				   clientId: app.clientId},
			success: function(data){x
				console.log(data)
			},
			dataType: 'json'
			});
	
		return false;

	}



	app.updateFPS = function() {
		app.selectors.fps.html(Math.floor(1000/app.elapsed));
	}

	app.toggleOverlay = function() {

		app.displayFaceOverlay = !app.displayFaceOverlay;

	}


	app.setup = function() {

		// resize overlay to match canvas size
		window.setInterval(app.sendData,1000);
		
		delete emotionModel['disgusted'];
		delete emotionModel['fear'];
		delete emotionModel['angry'];
		delete emotionModel['surprised']; 
		app.ec = new emotionClassifier();
		app.ec.init(emotionModel);
		var emotionData = app.ec.getBlank();

		app.selectors.enableFaceOverlay.on('click',app.toggleOverlay);
		

	}

	app.drawPositions = function(positions) {

		drawing = true;

		var drawingContext = overlayCC;

		overlayCC.strokeStyle = "#FFF";
		overlayCC.lineWidth = 10;

	
		// set the color of the ellipse based on position on screen
		//fill(map(positions[i][0], width*0.33, width*0.66, 0, 255), map(positions[i][1], height*0.33, height*0.66, 0, 255), 255);
		// draw ellipse at each position point

		let d = dist(positions[62][0], positions[62][1], positions[33][0], positions[33][1]);
		var rectSize = d/10;
		
	//	strokeWeight(d/50);
		//console.log(positions[0][0]);
	//	stroke(0);

		drawingContext.setLineDash([2, 5]);
		
		// nose
		line(positions[23][0], positions[23][1], positions[34][0], positions[34][1]);
		line(positions[28][0], positions[28][1], positions[40][0], positions[40][1]);
		line(positions[40][0], positions[40][1], positions[39][0], positions[39][1]);
		line(positions[34][0], positions[34][1], positions[35][0], positions[35][1]);
		line(positions[50][0], positions[50][1], positions[59][0], positions[59][1]);
		line(positions[44][0], positions[44][1], positions[61][0], positions[61][1]);
		line(positions[44][0], positions[44][1], positions[55][0], positions[55][1]);
		line(positions[50][0], positions[50][1], positions[51][0], positions[51][1]);
		line(positions[54][0], positions[54][1], positions[56][0], positions[56][1]);
		line(positions[53][0], positions[53][1], positions[57][0], positions[57][1]);
		line(positions[52][0], positions[52][1], positions[58][0], positions[58][1]);
		line(positions[53][0], positions[53][1], positions[7][0], positions[7][1]);
		line(positions[37][0], positions[37][1], positions[47][0], positions[47][1]);
		line(positions[25][0], positions[25][1], positions[33][0], positions[33][1]);
		line(positions[30][0], positions[30][1], positions[33][0], positions[33][1]);

		// Right eye
		line(positions[50][0], positions[50][1], positions[58][0], positions[58][1]);
		line(positions[33][0], positions[33][1], positions[41][0], positions[41][1]);
		line(positions[44][0], positions[44][1], positions[56][0], positions[56][1]);
		line(positions[41][0], positions[41][1], positions[62][0], positions[62][1]);
		line(positions[23][0], positions[23][1], positions[63][0], positions[63][1]);
		line(positions[63][0], positions[63][1], positions[24][0], positions[24][1]);
		line(positions[24][0], positions[24][1], positions[64][0], positions[64][1]);
		line(positions[64][0], positions[64][1], positions[25][0], positions[25][1]);
		line(positions[25][0], positions[25][1], positions[65][0], positions[65][1]);
		line(positions[65][0], positions[65][1], positions[26][0], positions[26][1]);
		line(positions[26][0], positions[26][1], positions[66][0], positions[66][1]);
		line(positions[66][0], positions[66][1], positions[23][0], positions[23][1]);

		// Left eye
		line(positions[30][0], positions[30][1], positions[68][0], positions[68][1]);
		line(positions[29][0], positions[29][1], positions[68][0], positions[68][1]);
		line(positions[29][0], positions[29][1], positions[67][0], positions[67][1]);
		line(positions[28][0], positions[28][1], positions[67][0], positions[67][1]);
		line(positions[28][0], positions[28][1], positions[70][0], positions[70][1]);
		line(positions[31][0], positions[31][1], positions[70][0], positions[70][1]);
		line(positions[31][0], positions[31][1], positions[69][0], positions[69][1]);
		line(positions[30][0], positions[30][1], positions[69][0], positions[69][1]);
		line(positions[37][0], positions[37][1], positions[47][0], positions[47][1]);

		for(i=0; i<60; i++) {

			// left cheek
			if (i > 10 && i < 14) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}
			// right cheek
			if (i > -1 && i < 3) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}
			// lower upper lip
			if (i > 58 && i < 61) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}

			drawingContext.setLineDash([1]);

			//upper upper lip
			if (i > 43 && i < 50) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}
			if (i > 50 && i < 55) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}

			if (i > 2 && i < 11) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}

			if (i > 18 && i < 22) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}

			if (i > 14 && i < 18) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}
			if (i > 35 && i < 38) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}
			if (i > 55 && i < 58) {
			line(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1]);
			}
		}
		

		//rectangles
		
	


		drawing = false;
	}

	app.setup();

	$('#start').on('click',startVideo);


})