(function ($) {
	
	"use strict";

	 $(window).on('load', function() {
    $('#call-screen').hide();
    $('#incoming-screen').hide();
    
    const socket = new WebSocket('ws://localhost:12345');

    socket.addEventListener('open', () => {
      console.log('Connected to server');

      // Send data to the server
      // socket.send('Hello from client');
    });

    // Listen for messages from the server
    socket.addEventListener('message', (event) => {
      console.log('Message received from server:', event.data);
      var msg = JSON.parse(event.data);
      if(msg.msg_type === "callscreen"){
        if(msg.value === true){
          $('#call-screen').show();
        } else {
          $('#call-screen').hide();
        }
      } else if(msg.msg_type === "incomingscreen"){
        if(msg.value === true){
          $('#incoming-screen').show();
        } else {
          $('#incoming-screen').hide();
        }
      }
    });

    // Connection closed
    socket.addEventListener('close', () => {
      console.log('Connection closed');
    });

    $('#subVoice').on('click', function() {
      var phNumber = $('#phNumber').val();
      if(phNumber == ""){
        alert('Please input phone number');
      } else {
        var msg_call = {
          msg_type: 'voicecall',
          phone_number: phNumber
        }
        socket.send(JSON.stringify(msg_call));
      }
    });
    $('#callReject').on('click', function() {
      var msg_call = {
        msg_type: 'callreject',
      }
      socket.send(JSON.stringify(msg_call));
    });
    $('#incomeAccept').on('click', function() {
      var msg_call = {
        msg_type: 'incomeaccept',
      }
      socket.send(JSON.stringify(msg_call));
    });
    $('#incomeReject').on('click', function() {
      var msg_call = {
        msg_type: 'incomereject',
      }
      socket.send(JSON.stringify(msg_call));
    });
  });

})(window.jQuery);