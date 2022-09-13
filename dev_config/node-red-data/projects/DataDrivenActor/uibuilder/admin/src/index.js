/* jshint browser: true, esversion: 5, asi: true */
/*globals Vue, uibuilder  */
// @ts-nocheck
/*
  Copyright (c) 2019 Julian Knight (Totally Information)

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
'use strict'

/** @see https://github.com/TotallyInformation/node-red-contrib-uibuilder/wiki/Front-End-Library---available-properties-and-methods */

// eslint-disable-next-line no-unused-vars
var app1 = new Vue({
    el: '#app',
    data: {
        face1: null,
        face_phones:[{
            'whole': 1,
            'left eyebrow': 1,
            'right eyebrow': 1,
            'left eye': 1,
            'right eye': 1,
            'mouth': 1,
            },
            {
            'whole': 1,
            'left eyebrow': 1,
            'right eyebrow': 1,
            'left eye': 1,
            'right eye': 1,
            'mouth': 1,
            }
        ],
        data: ['emotion', 'emotion'],
        data_options: [
          { value: 'emotion', text: 'Emotion' },
          { value: 'dataset', text: 'Dataset' }
        ],
        continuous: [false, false],
        processing_options: [
          { value: true, text: 'Continuous' },
          { value: false, text: 'Discrete' }
        ],
        face_features: [false, false],
        socketConnectedState : false,
        serverTimeOffset     : '[unknown]',
        msgRecvd    : '[Nothing]',
        msgsReceived: 0,
        msgCtrl     : '[Nothing]',
        msgsControl : 0,
        msgSent     : '[Nothing]',
        msgsSent    : 0,
        msgCtrlSent : '[Nothing]',
        msgsCtrlSent: 0,
    }, // --- End of data --- //
    computed: {
        hLastRcvd: function() {
            var msgRecvd = this.msgRecvd
            this.mappings = msgRecvd.mappings;
            if (typeof msgRecvd === 'string') return 'Last Message Received = ' + msgRecvd
            else return 'Last Message Received = ' + this.syntaxHighlight(msgRecvd)
        },
        hLastSent: function() {
            var msgSent = this.msgSent
            if (typeof msgSent === 'string') return 'Last Message Sent = ' + msgSent
            else return 'Last Message Sent = ' + this.syntaxHighlight(msgSent)
        },
        hLastCtrlRcvd: function() {
            var msgCtrl = this.msgCtrl
            if (typeof msgCtrl === 'string') return 'Last Control Message Received = ' + msgCtrl
            else return 'Last Control Message Received = ' + this.syntaxHighlight(msgCtrl)
        },
        hLastCtrlSent: function() {
            var msgCtrlSent = this.msgCtrlSent
            if (typeof msgCtrlSent === 'string') return 'Last Control Message Sent = ' + msgCtrlSent
            //else return 'Last Message Sent = ' + this.callMethod('syntaxHighlight', [msgCtrlSent])
            else return 'Last Control Message Sent = ' + this.syntaxHighlight(msgCtrlSent)
        },
    }, // --- End of computed --- //
    methods: {
        sendData: function() {
            uibuilder.send( {
                'topic': 'admin_change',
                'mappings': this.mappings,
                'data_source': this.data,
                'continuous': this.continuous,
                'face_features': this.face_features
            } )
        },
        faceChange: function(face) {
            for (let [key, value] of Object.entries(this.face_phones[face-1])) {
                // set all features to the same phone
                this.face_phones[face-1][key] = this.face_phones[face-1]['whole'];
            }
            for (let [key, value] of Object.entries(this.mappings)) {
                // update mappings
                if (value.face === face){
                    this.mappings[key].phone = this.face_phones[face-1]['whole'];
                }
            }
            this.sendData();
        },
        featureChange: function(face, feature) {
            for (let [key, value] of Object.entries(this.mappings)) {
                if (value.face === face && value.feature === feature){
                    this.mappings[key].phone = this.face_phones[face-1][feature];
                }
            }
            this.sendData();
        },
        faceToggle1: function(val) {
            this.face_features[0] = val;
            if (this.face_features[0]) {
                for (let [map_key, map_value] of Object.entries(this.mappings)) {
                    if (map_value.face === 1){
                        this.mappings[map_key].phone = this.face_phones[0]['whole'];
                    }
                }
            }
            this.sendData();
        },
        faceToggle2: function(val) {
            this.face_features[1] = val;
            if (this.face_features[1]) {
                for (let [map_key, map_value] of Object.entries(this.mappings)) {
                    if (map_value.face === 2){
                        this.mappings[map_key].phone = this.face_phones[1]['whole'];
                    }
                }
            }
            this.sendData();
        },
        dataRadio1: function(val) {
            this.data[0] = val;
            this.sendData();    
        },
        continuousRadio1: function(val) {
            this.continuous[0] = val;
            this.sendData();    
        },
        dataRadio2: function(val) {
            this.data[1] = val;
            this.sendData();    
        },
        continuousRadio2: function(val) {
            this.continuous[1] = val;
            this.sendData();    
        },
    }, // --- End of methods --- //

    // Available hooks: init,mounted,updated,destroyed
    mounted: function(){
        //console.debug('[indexjs:Vue.mounted] app mounted - setting up uibuilder watchers')

        /** **REQUIRED** Start uibuilder comms with Node-RED @since v2.0.0-dev3
         * Pass the namespace and ioPath variables if hosting page is not in the instance root folder
         * e.g. If you get continual `uibuilderfe:ioSetup: SOCKET CONNECT ERROR` error messages.
         * e.g. uibuilder.start('/nr/uib', '/nr/uibuilder/vendor/socket.io') // change to use your paths/names
         */
        uibuilder.start()

        var vueApp = this

        // Example of retrieving data from uibuilder
        vueApp.feVersion = uibuilder.get('version')

        /** You can use the following to help trace how messages flow back and forth.
         * You can then amend this processing to suite your requirements.
         */

        //#region ---- Trace Received Messages ---- //
        // If msg changes - msg is updated when a standard msg is received from Node-RED over Socket.IO
        // newVal relates to the attribute being listened to.
        uibuilder.onChange('msg', function(newVal){
            // console.info('[indexjs:uibuilder.onChange] msg received from Node-RED server:', newVal)
            vueApp.msgRecvd = newVal
            if (typeof newVal.mappings != 'undefined'){
                console.info('Got mappings')
                vueApp.mappings = newVal.mappings
                // update phone assignments from mappings
                for (let [key, value] of Object.entries(newVal.mappings)) {
                    vueApp.face_phones[value.face - 1][value.feature] = value.phone;
                    vueApp.face_phones[value.face - 1]['whole'] = value.phone;
                }
            }
            
            if (typeof newVal.data_source != 'undefined'){
                vueApp.data = newVal.data_source;
            }
            
            if (typeof newVal.continuous != 'undefined'){
                vueApp.continuous = newVal.continuous;
            }
            
            if (typeof newVal.face_features != 'undefined'){
                vueApp.face_features = newVal.face_features;
            }
                
        })
        // As we receive new messages, we get an updated count as well
        uibuilder.onChange('msgsReceived', function(newVal){
            //console.info('[indexjs:uibuilder.onChange] Updated count of received msgs:', newVal)
            vueApp.msgsReceived = newVal
        })

        // If we receive a control message from Node-RED, we can get the new data here - we pass it to a Vue variable
        uibuilder.onChange('ctrlMsg', function(newVal){
            //console.info('[indexjs:uibuilder.onChange:ctrlMsg] CONTROL msg received from Node-RED server:', newVal)
            vueApp.msgCtrl = newVal
        })
        // Updated count of control messages received
        uibuilder.onChange('msgsCtrl', function(newVal){
            //console.info('[indexjs:uibuilder.onChange:msgsCtrl] Updated count of received CONTROL msgs:', newVal)
            vueApp.msgsControl = newVal
        })
        //#endregion ---- End of Trace Received Messages ---- //

        //#region ---- Trace Sent Messages ---- //
        // You probably only need these to help you understand the order of processing //
        // If a message is sent back to Node-RED, we can grab a copy here if we want to
        uibuilder.onChange('sentMsg', function(newVal){
            //console.info('[indexjs:uibuilder.onChange:sentMsg] msg sent to Node-RED server:', newVal)
            vueApp.msgSent = newVal
        })
        // Updated count of sent messages
        uibuilder.onChange('msgsSent', function(newVal){
            //console.info('[indexjs:uibuilder.onChange:msgsSent] Updated count of msgs sent:', newVal)
            vueApp.msgsSent = newVal
        })

        // If we send a control message to Node-RED, we can get a copy of it here
        uibuilder.onChange('sentCtrlMsg', function(newVal){
            //console.info('[indexjs:uibuilder.onChange:sentCtrlMsg] Control message sent to Node-RED server:', newVal)
            vueApp.msgCtrlSent = newVal
        })
        // And we can get an updated count
        uibuilder.onChange('msgsSentCtrl', function(newVal){
            //console.info('[indexjs:uibuilder.onChange:msgsSentCtrl] Updated count of CONTROL msgs sent:', newVal)
            vueApp.msgsCtrlSent = newVal
        })
        //#endregion ---- End of Trace Sent Messages ---- //

        // If Socket.IO connects/disconnects, we get true/false here
        uibuilder.onChange('ioConnected', function(newVal){
            //console.info('[indexjs:uibuilder.onChange:ioConnected] Socket.IO Connection Status Changed to:', newVal)
            vueApp.socketConnectedState = newVal
        })
        // If Server Time Offset changes
        uibuilder.onChange('serverTimeOffset', function(newVal){
            //console.info('[indexjs:uibuilder.onChange:serverTimeOffset] Offset of time between the browser and the server has changed to:', newVal)
            vueApp.serverTimeOffset = newVal
        })

    } // --- End of mounted hook --- //

}) // --- End of app1 --- //

// EOF