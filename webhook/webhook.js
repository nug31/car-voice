/**
 * Google Assistant Webhook for Car Voice Control
 * This webhook handles requests from your Google Assistant project
 */

const express = require('express');
const { conversation } = require('@assistant/conversation');
const app = express();

// Initialize the conversation app
const conv = conversation({
    debug: true
});

// Car control functions (integrate with your existing CarDataManager)
class CarController {
    constructor() {
        this.carData = {
            engineRunning: false,
            batteryLevel: 85,
            fuelLevel: 67,
            engineTemp: 195,
            engineRPM: 0,
            vehicleSpeed: 0,
            mileage: 45230
        };
    }

    startEngine() {
        this.carData.engineRunning = true;
        this.carData.engineRPM = 800;
        return "Engine started successfully! Your car is now running.";
    }

    stopEngine() {
        this.carData.engineRunning = false;
        this.carData.engineRPM = 0;
        this.carData.vehicleSpeed = 0;
        return "Engine stopped safely! Your car is now off.";
    }

    getCarStatus() {
        const status = this.carData.engineRunning ? 'running' : 'stopped';
        return `Your car engine is currently ${status}. Battery: ${this.carData.batteryLevel}%, Fuel: ${this.carData.fuelLevel}%`;
    }

    getBatteryLevel() {
        return `Your battery level is ${this.carData.batteryLevel} percent.`;
    }

    getFuelLevel() {
        return `Your fuel level is ${this.carData.fuelLevel} percent.`;
    }

    openTrunk() {
        return "Trunk opened successfully!";
    }

    closeTrunk() {
        return "Trunk closed successfully!";
    }

    async activateAllFeatures() {
        let response = "Starting automated sequence. ";
        
        // Start engine
        this.startEngine();
        response += "Engine started. ";
        
        // Simulate trunk operations
        response += "Trunk opened and closed. ";
        
        // Get status
        response += this.getCarStatus() + " ";
        
        // Stop engine
        this.stopEngine();
        response += "Engine stopped. Sequence complete!";
        
        return response;
    }
}

const carController = new CarController();

// Welcome intent
conv.handle('welcome', conv => {
    conv.add('Hello! I am Car Voice. I can help you control your car with voice commands. You can ask me to start your car, check status, or run automated sequences.');
});

// Start car intent
conv.handle('start_car', conv => {
    const response = carController.startEngine();
    conv.add(response);
});

// Stop car intent
conv.handle('stop_car', conv => {
    const response = carController.stopEngine();
    conv.add(response);
});

// Check car status intent
conv.handle('check_status', conv => {
    const response = carController.getCarStatus();
    conv.add(response);
});

// Check battery level intent
conv.handle('check_battery', conv => {
    const response = carController.getBatteryLevel();
    conv.add(response);
});

// Check fuel level intent
conv.handle('check_fuel', conv => {
    const response = carController.getFuelLevel();
    conv.add(response);
});

// Open trunk intent
conv.handle('open_trunk', conv => {
    const response = carController.openTrunk();
    conv.add(response);
});

// Close trunk intent
conv.handle('close_trunk', conv => {
    const response = carController.closeTrunk();
    conv.add(response);
});

// Active all sequence intent
conv.handle('active_all', async conv => {
    const response = await carController.activateAllFeatures();
    conv.add(response);
});

// Help intent
conv.handle('help', conv => {
    conv.add('I can help you with these commands: start car, stop car, check car status, check battery level, check fuel level, open trunk, close trunk, or run active all sequence. What would you like to do?');
});

// Default fallback intent
conv.handle('default_fallback', conv => {
    conv.add('I didn\'t understand that command. You can ask me to start your car, check status, or say help for more options.');
});

// Express middleware
app.use(express.json());

// Webhook endpoint
app.post('/webhook', conv);

// Health check endpoint
app.get('/health', (req, res) => {
    res.json({ status: 'OK', message: 'Car Voice webhook is running' });
});

// Start server
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`🚗 Car Voice webhook server running on port ${PORT}`);
    console.log(`📡 Webhook URL: http://localhost:${PORT}/webhook`);
    console.log(`🏥 Health check: http://localhost:${PORT}/health`);
});

module.exports = app;
