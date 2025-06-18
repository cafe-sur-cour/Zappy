#!/bin/bash

# Script to diagnose and fix plugin-related issues in Jenkins

echo "==== JENKINS PLUGIN DIAGNOSTICS ===="
echo ""

# Check if running in Jenkins environment
if [ -z "$JENKINS_HOME" ]; then
    echo "Not running in Jenkins environment"
    exit 1
fi

# Function to check if a plugin is installed
check_plugin() {
    local plugin_name="$1"
    if [ -d "$JENKINS_HOME/plugins/$plugin_name" ]; then
        echo "✓ Plugin '$plugin_name' is installed"
        return 0
    else
        echo "✗ Plugin '$plugin_name' is NOT installed"
        return 1
    fi
}

# Check essential plugins
echo "Checking for required plugins:"
check_plugin "postbuildscript"
check_plugin "copyartifact"
check_plugin "workflow-aggregator"
check_plugin "job-dsl"
check_plugin "git"

echo ""
echo "Checking plugin versions:"
if [ -f "$JENKINS_HOME/plugins/postbuildscript/META-INF/MANIFEST.MF" ]; then
    VERSION=$(grep "Plugin-Version" "$JENKINS_HOME/plugins/postbuildscript/META-INF/MANIFEST.MF" | cut -d' ' -f2)
    echo "PostBuildScript plugin version: $VERSION"
fi

if [ -f "$JENKINS_HOME/plugins/copyartifact/META-INF/MANIFEST.MF" ]; then
    VERSION=$(grep "Plugin-Version" "$JENKINS_HOME/plugins/copyartifact/META-INF/MANIFEST.MF" | cut -d' ' -f2)
    echo "CopyArtifact plugin version: $VERSION"
fi

echo ""
echo "==== PLUGIN DIAGNOSTIC COMPLETE ===="
exit 0
