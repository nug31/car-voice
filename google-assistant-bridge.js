/**
 * Google Assistant Bridge
 * Connects the webhook with the existing web app's CarDataManager
 */

// This script can be included in your web app to sync with Google Assistant
class GoogleAssistantBridge {
    constructor(carDataManager) {
        this.carDataManager = carDataManager;
        this.webhookUrl = 'http://localhost:3000'; // Update with your webhook URL
        this.isConnected = false;
        this.init();
    }

    async init() {
        try {
            // Check if webhook is available
            const response = await fetch(`${this.webhookUrl}/health`);
            if (response.ok) {
                this.isConnected = true;
                console.log('🎤 Google Assistant webhook connected');
                this.startSync();
            }
        } catch (error) {
            console.log('🔇 Google Assistant webhook not available');
        }
    }

    // Sync car data with webhook
    startSync() {
        setInterval(() => {
            this.syncCarData();
        }, 5000); // Sync every 5 seconds
    }

    async syncCarData() {
        if (!this.isConnected) return;

        try {
            // Send current car data to webhook
            await fetch(`${this.webhookUrl}/sync`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    carData: this.carDataManager.realTimeData,
                    connections: this.carDataManager.connections
                })
            });
        } catch (error) {
            console.error('Sync error:', error);
        }
    }

    // Handle commands from Google Assistant
    async handleAssistantCommand(command, parameters = {}) {
        switch (command) {
            case 'start_car':
                return this.carDataManager.startEngine();
            
            case 'stop_car':
                return this.carDataManager.stopEngine();
            
            case 'check_status':
                return this.carDataManager.getCarStatus();
            
            case 'check_battery':
                const batteryLevel = Math.round(this.carDataManager.realTimeData.batteryLevel);
                return `Battery level is ${batteryLevel} percent.`;
            
            case 'check_fuel':
                const fuelLevel = Math.round(this.carDataManager.realTimeData.fuelLevel);
                return `Fuel level is ${fuelLevel} percent.`;
            
            case 'open_trunk':
                return "Trunk opened successfully!";
            
            case 'close_trunk':
                return "Trunk closed successfully!";
            
            case 'active_all':
                // Use the existing activateAllFeatures if available
                if (typeof aiVoice !== 'undefined' && aiVoice.activateAllFeatures) {
                    await aiVoice.activateAllFeatures();
                    return "Automated sequence completed successfully!";
                } else {
                    // Fallback implementation
                    this.carDataManager.startEngine();
                    await this.delay(2000);
                    const status = this.carDataManager.getCarStatus();
                    this.carDataManager.stopEngine();
                    return `Sequence complete: ${status}`;
                }
            
            default:
                return "Command not recognized. Available commands: start car, stop car, check status, check battery, check fuel, open trunk, close trunk, active all.";
        }
    }

    delay(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }

    // Update webhook URL (for production deployment)
    updateWebhookUrl(newUrl) {
        this.webhookUrl = newUrl;
        this.init(); // Reconnect
    }

    // Get connection status
    getStatus() {
        return {
            connected: this.isConnected,
            webhookUrl: this.webhookUrl,
            lastSync: new Date().toISOString()
        };
    }
}

// Auto-initialize if CarDataManager is available
if (typeof carDataManager !== 'undefined') {
    window.googleAssistantBridge = new GoogleAssistantBridge(carDataManager);
    
    // Add status indicator to the page
    const statusIndicator = document.createElement('div');
    statusIndicator.id = 'google-assistant-status';
    statusIndicator.style.cssText = `
        position: fixed;
        top: 80px;
        right: 20px;
        background: rgba(0,0,0,0.8);
        color: white;
        padding: 10px;
        border-radius: 8px;
        font-size: 12px;
        z-index: 1001;
        display: none;
    `;
    document.body.appendChild(statusIndicator);

    // Show status when connected
    setTimeout(() => {
        const bridge = window.googleAssistantBridge;
        if (bridge.isConnected) {
            statusIndicator.innerHTML = '🎤 Google Assistant Connected';
            statusIndicator.style.background = 'rgba(52, 168, 83, 0.9)';
            statusIndicator.style.display = 'block';
            
            // Hide after 3 seconds
            setTimeout(() => {
                statusIndicator.style.display = 'none';
            }, 3000);
        }
    }, 2000);
}

// Export for use in other modules
if (typeof module !== 'undefined' && module.exports) {
    module.exports = GoogleAssistantBridge;
}
