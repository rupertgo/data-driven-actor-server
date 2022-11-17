var socket = new WebSocket("ws://192.168.4.1:1880/ws/digits");

socket.onopen = function(e) {
	socket.send('12');
	console.log('alert');
}

socket.onerror = function(error) {
	alert(`[error] ${error.message}`);
  };