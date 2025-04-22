pipeline {
    agent any

    environment {
        // Define directories for the node packages
        NODE_PACKAGE_DIR = 'node_package'  // Directory for BLN node package
        EL_PACKAGE_DIR = 'el_package'      // Directory for EL package
        BLN_PACKAGE_NAME = 'bln_node_package.zip'  // BLN node package name
        EL_PACKAGE_NAME = 'el_node_package.zip'    // EL package name (adjust as needed)
    }

    stages {
        stage('Checkout SCM') {
            steps {
                checkout scm
            }
        }

        stage('Build Node Packages') {
            steps {
                script {
                    // List contents of the current directory for debugging
                    echo "Listing current directory contents..."
                    bat 'dir'

                    // Check if the label file exists for BLN
                    echo "Checking if label file exists in the current directory..."
                    if (fileExists("label")) {
                        echo "Label file found, copying to ${NODE_PACKAGE_DIR}"
                        bat "if not exist ${NODE_PACKAGE_DIR} mkdir ${NODE_PACKAGE_DIR}"
                        bat "copy label ${NODE_PACKAGE_DIR}\\"
                    } else {
                        error "Label file not found in the workspace!"
                    }

                    // Assuming the EL package creation steps (e.g., generating dfd.xml & desc.asn)
                    echo "Building EL Package..."
                    bat """
                        if not exist ${EL_PACKAGE_DIR} mkdir ${EL_PACKAGE_DIR}
                        echo Creating EL node package...
                        # Replace with actual steps to build the EL package
                        # Example: el_build_script.bat (if you have such a script)
                        # Assuming it's a custom script or tool to create the EL package
                        build_el_package.bat
                    """

                    // Verify if the EL package is created
                    if (!fileExists("${EL_PACKAGE_DIR}\\${EL_PACKAGE_NAME}")) {
                        error "EL Node Package not found!"
                    }

                    // Example: After the EL package is created, you can also verify the BLN package
                    if (!fileExists("${NODE_PACKAGE_DIR}\\${BLN_PACKAGE_NAME}")) {
                        error "BLN Node Package not found!"
                    }
                }
            }
        }

        stage('Archive Package') {
            steps {
                archiveArtifacts artifacts: "${NODE_PACKAGE_DIR}/**/*, ${EL_PACKAGE_DIR}/**/*", allowEmptyArchive: true
            }
        }

        stage('Clean Workspace') {
            steps {
                cleanWs()
            }
        }
    }

    post {
        always {
            echo "Cleaning up after pipeline execution"
        }

        success {
            echo "Pipeline executed successfully"
        }

        failure {
            echo "Pipeline failed"
        }
    }
}
