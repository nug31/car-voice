# 🎤 Google Assistant Integration Setup

Since you already have a Google Assistant project, here's how to connect it with your Car Voice application.

## 🔧 Quick Setup Steps

### 1. 📦 Install Dependencies
```bash
npm install
```

### 2. 🚀 Start the Webhook Server
```bash
npm run webhook
```
Your webhook will be available at: `http://localhost:3000/webhook`

### 3. 🌐 Make Webhook Accessible (Choose One)

#### Option A: ngrok (Recommended for Testing)
```bash
# Install ngrok
npm install -g ngrok

# Expose your local webhook
ngrok http 3000
```
Copy the HTTPS URL (e.g., `https://abc123.ngrok.io/webhook`)

#### Option B: Deploy to Google Cloud Functions
```bash
# Install Google Cloud CLI first
npm run deploy-webhook
```

### 4. 🔗 Update Your Google Assistant Project

1. **Go to Actions Console**: https://console.actions.google.com
2. **Select your existing project**
3. **Go to Webhook settings**
4. **Update Fulfillment URL** to your webhook URL:
   - ngrok: `https://abc123.ngrok.io/webhook`
   - Cloud Functions: `https://region-project.cloudfunctions.net/carVoiceWebhook`

### 5. 🎯 Configure Intents (If Not Already Done)

Your webhook handles these intents:
- `welcome` - Welcome message
- `start_car` - Start the car engine
- `stop_car` - Stop the car engine  
- `check_status` - Get car status
- `check_battery` - Check battery level
- `check_fuel` - Check fuel level
- `open_trunk` - Open trunk (buka bagasi)
- `close_trunk` - Close trunk (tutup bagasi)
- `active_all` - Run automated sequence
- `help` - Show available commands
- `default_fallback` - Handle unknown commands

### 6. 🗣️ Sample Training Phrases

Add these to your intents in Actions Console:

**start_car intent:**
- "start my car"
- "turn on my car"
- "start the engine"
- "start my vehicle"

**stop_car intent:**
- "stop my car"
- "turn off my car"
- "stop the engine"
- "turn off my vehicle"

**check_status intent:**
- "check my car status"
- "how is my car"
- "car status"
- "vehicle status"

**check_battery intent:**
- "check battery level"
- "what's my battery level"
- "battery status"
- "how much battery"

**check_fuel intent:**
- "check fuel level"
- "how much fuel"
- "fuel status"
- "gas level"

**open_trunk intent:**
- "open trunk"
- "open the trunk"
- "buka bagasi"

**close_trunk intent:**
- "close trunk"
- "close the trunk"
- "tutup bagasi"

**active_all intent:**
- "active all"
- "run all commands"
- "automated sequence"
- "do everything"

## 🧪 Testing Your Integration

### 1. Test Webhook Directly
```bash
curl -X POST http://localhost:3000/webhook \
  -H "Content-Type: application/json" \
  -d '{"handler":{"name":"start_car"},"intent":{"name":"start_car"}}'
```

### 2. Test with Google Assistant
- "Hey Google, talk to [Your App Name]"
- "Hey Google, ask [Your App Name] to start my car"
- "Hey Google, tell [Your App Name] to check car status"

### 3. Health Check
Visit: `http://localhost:3000/health`

## 🔄 Integration with Existing Car Data

The webhook includes a `CarController` class that mirrors your web app's functionality. To connect it with real car data:

1. **Replace simulation with real data sources**
2. **Add OBD-II integration**
3. **Connect to manufacturer APIs**
4. **Sync with your web dashboard**

## 🚀 Production Deployment

### Google Cloud Functions (Recommended)
```bash
# Deploy webhook
gcloud functions deploy carVoiceWebhook \
  --runtime nodejs18 \
  --trigger-http \
  --allow-unauthenticated \
  --source .
```

### Alternative: Heroku, AWS Lambda, etc.
The webhook is a standard Express.js app and can be deployed anywhere.

## 🔐 Security Considerations

1. **Add authentication** for production
2. **Validate requests** from Google Assistant
3. **Use HTTPS** for all webhook URLs
4. **Secure car control endpoints**

## 🎯 Voice Commands Available

Once integrated, users can say:
- "Hey Google, start my car"
- "Hey Google, stop my car"
- "Hey Google, check my car status"
- "Hey Google, what's my battery level"
- "Hey Google, open trunk"
- "Hey Google, active all"

## 🐛 Troubleshooting

1. **Webhook not responding**: Check if server is running on port 3000
2. **Google Assistant not connecting**: Verify webhook URL in Actions Console
3. **Commands not working**: Check intent names match webhook handlers
4. **HTTPS required**: Use ngrok or deploy to cloud for HTTPS

## 📞 Support

Your Google Assistant project is now connected to your Car Voice application! 🚗🎤✨
