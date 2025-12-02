#!/usr/bin/env bash
set -e

# Configuration
WORKFLOW_NAME="phase0a_harvest.yml"
ARTIFACT_NAME="phase0a_msvc_artifacts"
# Default to /tmp to avoid dropping binaries in the repo; override with LOCAL_DIR env if needed.
LOCAL_DIR="${LOCAL_DIR:-/tmp/phase0a_msvc_artifacts}"

echo "🔍 Looking for the most recent run of $WORKFLOW_NAME..."

# 1. Get the latest Run ID for the specific workflow
RUN_ID=$(gh run list --workflow "$WORKFLOW_NAME" --limit 1 --json databaseId -q '.[0].databaseId')

if [ -z "$RUN_ID" ]; then
    echo "❌ No runs found for workflow $WORKFLOW_NAME"
    exit 1
fi

echo "📋 Found Run ID: $RUN_ID"
echo "⏳ Waiting for run to complete..."

# 2. Wait for the run to finish (using GitHub CLI's built-in watch)
gh run watch "$RUN_ID" --exit-status

echo "✅ Run completed successfully."
echo "📦 Downloading artifacts..."

# 3. Download and Unzip
rm -rf "$LOCAL_DIR"
mkdir -p "$LOCAL_DIR"
gh run download "$RUN_ID" -n "$ARTIFACT_NAME" -D "$LOCAL_DIR"

echo "📂 Artifacts downloaded to $LOCAL_DIR/"
echo "---------------------------------------------------"
echo "🔎 INSPECTION RESULTS (Phase 0A - Stage 1)"
echo "---------------------------------------------------"

# 4. Inspect Layout (The "Golden Reference" Check)
# We grep for the class name and the next few lines to show the vptr
LAYOUT_FILE="$LOCAL_DIR/layout.log"
if [ -f "$LAYOUT_FILE" ]; then
    echo ">> Class Layout (from layout.log):"
    # Find the line with class CStage1_Simple, then print it and the next 5 lines
    grep -A 5 "class CStage1_Simple" "$LAYOUT_FILE" || echo "❌ Class layout not found in log!"
else
    echo "❌ layout.log missing!"
fi

echo "---------------------------------------------------"

# 5. Inspect Exports (The Mangling Check)
EXPORTS_FILE="$LOCAL_DIR/exports.txt"
if [ -f "$EXPORTS_FILE" ]; then
    echo ">> Mangled Name for GetValue (from exports.txt):"
    # Search for GetValue and print the mangled name
    grep "GetValue" "$EXPORTS_FILE" | awk '{print $4}' || echo "❌ GetValue symbol not found!"
else
    echo "❌ exports.txt missing!"
fi

echo "---------------------------------------------------"
