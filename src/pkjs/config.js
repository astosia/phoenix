// Clay Config: see https://github.com/pebble/clay
module.exports = [
  {
    "type": "heading",
    "defaultValue": "Settings"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Theme settings"
      },
      {
        "type": "color",
        "messageKey": "Back1Color",
        "defaultValue": "0x000000",
        "label": "Background"
      },
      {
        "type": "color",
        "messageKey": "HourColor",
        "defaultValue": "0x00FFFF",
        "label": "Hour Text"
      },
      {
        "type": "color",
        "messageKey": "MinColor",
        "defaultValue": "0xFFFFAA",
        "label": "Minute Text"
      }, 
      {
        "type": "color",
        "messageKey": "Text1Color",
        "defaultValue": "0xFFFFFF",
        "label": "Weather"
      },
       {
        "type": "color",
        "messageKey": "Text5Color",
        "defaultValue": "0xFFFFFF",
        "label": "Wind"
      }, 
      {
        "type": "color",
        "messageKey": "Text2Color",
        "defaultValue": "0xFFFFFF",
        "label": "Complications Left"
      }, 
       {
        "type": "color",
        "messageKey": "Text3Color",
        "defaultValue": "0xFFFFFF",
        "label": "Complications Right"
      }, 
      {
        "type":"color",
        "messageKey":"Text4Color",
        "defaultValue":"0xFFFFFF",
        "label":"Location"
      },
      {"type": "section",
       "items": [
         {
           "type": "heading",
           "defaultValue": "Night Theme",
           "size":4
         } ,
         {
           "type": "toggle",
           "messageKey": "NightTheme",
           "label": "Activate Night Theme",
           "defaultValue": false,        
         },
         {
           "type": "color",
           "messageKey": "Back1ColorN",
           "defaultValue": "0xFFFFFF",
           "label": "Background"
         },
         {
        "type": "color",
        "messageKey": "HourColorN",
        "defaultValue": "0x000000",
        "label": "Hour Text"
      },
      {
        "type": "color",
        "messageKey": "MinColorN",
        "defaultValue": "0x000000",
        "label": "Minute Text"
      }, 
         {
           "type": "color",
           "messageKey": "Text1ColorN",
           "defaultValue": "0x000000",
           "label": "Weather"
         }, 
       {
           "type": "color",
           "messageKey": "Text5ColorN",
           "defaultValue": "0x000000",
           "label": "Wind"
         }, 
         {
           "type": "color",
           "messageKey": "Text2ColorN",
           "defaultValue": "0x000000",
           "label": "Complications Left"
         },
          {
        "type": "color",
        "messageKey": "Text3ColorN",
        "defaultValue": "0x000000",
        "label": "Complications Right"
      }, 
         {
           "type":"color",
           "messageKey":"Text4ColorN",
           "defaultValue":"0x000000",
           "label":"Location"
         },
       ]
         }
       ]
      },
      {
        "type": "section",
        "items": [
          {
            "type": "heading",
            "defaultValue": "Weather settings"
          },     
          {
            "type": "toggle",
            "messageKey": "WeatherUnit",
            "label": "Temperature in Fahrenheit",
            "defaultValue": false,
          },
            {
            "type": "select",
            "messageKey": "WindUnit",
            "label": "Wind speed",
            "defaultValue": "knots",
               "options": [
              {
                "label": "knots",
                "value": "kts"
              },
              {
                "label": "miles per hour",
                "value": "mph"
              },
                 {
                "label": "metres per second",
                "value": "ms"
              },
              {
                "label": "kilometres per hour",
                "value": "kph"
              }
            ]
          },
          {
            "type": "select",
            "messageKey": "WeatherProv",
            "defaultValue": "owm",
            "label": "Weather Provider",
            "options": [
              {
                "label": "OpenWeatherMap",
                "value": "owm"
              },
              {
                "label": "WeatherUnderground",
                "value": "wu"
              },
              {
                "label": "DarkSky",
                "value": "ds"
              }
            ]
          },
          {
            "type": "input",
            "messageKey": "APIKEY_User",
            "defaultValue": "",
            "label": "API Key",
            "description": "If left blank, the watch will attempt to request an api from your pmkey.xyz.  If you don't have an api key, weather data will not be displayed. Note that DarkSky will show Latitude, Longitude (to 2 decimal places) instead of location name. Register for a free personal API key at <a href=https://darksky.net/dev/register/>DarkSky here</a>, or <a href =https://home.openweathermap.org/users/sign_up/>OpenWeatherMap here</a>",
            "attributes": {
              "placeholder": "Paste your API Key here"
            }
          },
          {
            "type": "input",
            "messageKey": "EmailPMKEY",
            "defaultValue": "",
            "label": "pmkey.xyz User",
            "description": "pmkey.xyz is a free service for Pebble users that allows you to safely store all your API keys in a single place. Check <a href=https://www.pmkey.xyz/>pmkey.xyz</a> ",
            "attributes": {
              "placeholder": "eg: jane.smith@pmkey.xyz",
              "type": "email"
            }
          },
          {
            "type": "input",
            "messageKey": "PINPMKEY",
            "defaultValue": "",
            "label": "pmkey.xyz PIN",
            "attributes": {
              "placeholder": "eg: 12345"
            }
          },
          {
            "type": "slider",
            "messageKey": "UpSlider",
            "defaultValue": 30,
            "label": "Update frequency (minutes)",
            "description": "More frequent requests will drain your phone battery faster",
            "min": 15,
            "max": 120,
            "step": 15},
        ]
          },
          {
          "type": "submit",
          "defaultValue":"SAVE"
          },
          {
          "type": "heading",
          "defaultValue": "version v2.0",
          "size":6
          },
          {
          "type": "heading",
          "defaultValue": "Made in UK",
          "size":6
          }
        ];