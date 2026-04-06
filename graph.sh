
#!/bin/bash

URL="http://esp32-55C971.fernie.local"

# Clear screen once at start
clear

while true; do
    # Fetch JSON and extract values (default to 0 if missing)
    JSON=$(curl -s --max-time 1 "$URL")
    
    ADC4=$(echo "$JSON" | jq -r '.ADC1_4 // 0')
    ADC5=$(echo "$JSON" | jq -r '.ADC1_5 // 0')
    ADC6=$(echo "$JSON" | jq -r '.ADC1_6 // 0')

    # Scale: assuming 12-bit ADC (0-4095) → bar width up to ~80 chars
    MAX_WIDTH=60
    SCALE=$((MAX_WIDTH * 1000 / 4095))  # rough scaling factor

    BAR4=$(printf '█%.0s' $(seq 1 $(((4095 - ADC4) * SCALE / 1000))))
    BAR5=$(printf '█%.0s' $(seq 1 $((ADC5 > 0 ? MAX_WIDTH : 0))))
    BAR6=$(printf '█%.0s' $(seq 1 $((ADC6 > 0 ? MAX_WIDTH : 0))))

    # Redraw everything
    clear
    echo "Polling $URL every 0.5s (Ctrl+C to stop)"
    echo "──────────────────────────────────────────────────────────────"
    printf "Left Pedal (ADC1_5):  %4d | %s\n" "$ADC5" "$BAR5"
    printf "Middle Pedal (ADC1_6):%4d | %s\n" "$ADC6" "$BAR6"
    printf "Right Pedal (ADC1_4): %4d | %s\n" "$ADC4" "$BAR4"
    echo "──────────────────────────────────────────────────────────────"
    echo "Values for Right Pedal are raw ADC readings (0–4095)"

    sleep 0.5
done
