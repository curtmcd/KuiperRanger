#!/bin/sh

# Create the system-wide high scores file
# (OK to run if there's already a high score file)

echo "Creating high score file..."
sudo mkdir -p /var/games/kuiper-ranger
sudo touch /var/games/kuiper-ranger/scores
sudo chmod 777 /var/games/kuiper-ranger/scores
echo "Done."
