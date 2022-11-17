jQuery(document).ready(function($) {

	app = {

		datasets: [
			{ id: 1,
			  name: 'leuchars weather',
			  data: 'weather/leuchars.html',
			  handler: "app.metofficeHandler",
			},
			{ id: 2,
			  name: 'london weather',
			  data: 'weather/london.html',
			  handler: "app.metofficeHandler",
			},
			
		],

		selectors: {
			loadData: $('#load-data'),
			datasetsSelect: $('#dataset-source'),
			datasetContainer: $('#dataset-container'),
			datasetTitle: $('#dataset-title'),
			playButton: $('.controls .button')
		},

		settings: {
			datasetsBaseUrl: 'datasets/',
			playSpeed: 250
		},

		currentDataset: null,
		playing: false,
		clientId: window.location.hash.substr(1)

	}

	app.metofficeHandler = function() {

		$row = $(this);

		$('.active-data').removeClass('active-data');
		$row.addClass('active-data');



		var sunLevel = $row.find('td').eq(2).text();
		
		app.sendData(parseFloat(sunLevel) / 250 * 100,0);

	}

	app.bindEvents = function() {

		app.selectors.loadData.on('click',app.loadDataset);
		app.selectors.playButton.on('click',app.togglePlay);

	}

	app.bindDatasetEvents = function() {

		/**  TO DO
		 *   generalize selector
		*/
		app.selectors.datasetContainer.on('click','tr',eval(app.currentDataset.handler));

	}

	app.loadDataset = function() {

		var $selectedOption = app.selectors.datasetsSelect.find('option:selected');

		var datasetId = $selectedOption.attr('data-id');

		app.datasets.forEach(element => {

			if(element.id == datasetId) {
				app.currentDataset = element;
			}

		});

		$.get(app.settings.datasetsBaseUrl+$selectedOption.val(), function(data) {

			app.selectors.datasetContainer.html(data)
			app.bindDatasetEvents();
			app.selectors.datasetTitle.text(app.currentDataset.name);

		})

		return false;

	}

	app.sendData = function(happy,sad) {

		data = {happy: happy,
				sad:	sad,
				clientId: app.clientId};

		console.log(data);
	
		$.ajax({
			type: "POST",
			url: "https://192.168.4.1:1880/data",
			data: data,
			success: function(data){
				console.log(data)
			},
			dataType: 'json'
			});
	
		return false;

	}

	app.populateDatasetsSelect = function() {

		app.datasets.forEach(element => {

			var $option = $('<option/>');

			$option.attr('value',element.data).html(element.name).attr('data-id',element.id);
			app.selectors.datasetsSelect.append($option);

		})

	}

	app.nextRecord = function() {

		if(app.playing) {
		
			var $activeRecord = $('tr.active-data');
	
			if($activeRecord.length == 0) {
				$nextRecord = app.selectors.datasetContainer.find('tr').eq(1);
			} else {
				$nextRecord = $activeRecord.next();
			}
	
			$activeRecord.removeClass('active-data');
	
			$nextRecord.addClass('active-data');
	
			$nextRecord.click();
		}

		

	}

	app.togglePlay = function() {
		
		app.selectors.playButton.toggleClass('paused');
		app.playing = !app.playing;

	}

	app.setup = function() {

		app.populateDatasetsSelect();
		app.bindEvents();
		
		window.setInterval(app.nextRecord,app.settings.playSpeed);

	}

	app.setup();

})