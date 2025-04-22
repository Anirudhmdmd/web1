pipeline {
    agent any

    environment {
        PACKAGE_DIR = 'node_package'  // Directory to store the package
        EL_PACKAGE_NAME = 'el_node_package.zip'  // Name of the EL node package
    }

    stages {
        stage('Checkout SCM') {
            steps {
                checkout scm
            }
        }

        stage('Build EL Node Package') {
            steps {
                script {
                    echo "Creating EL node package..."

                    // Check if the build_el_package.bat exists and is callable
                    echo "Checking if build_el_package.bat exists..."
                    bat "dir"

                    // Assuming build_el_package.bat exists in the current directory or a subdirectory
                    echo "Running EL package build script..."
                    bat "call build_el_package.bat"

                    // Verify if the EL package is created
                    if (!fileExists("${PACKAGE_DIR}\\${EL_PACKAGE_NAME}")) {
                        error "EL Node Package not found!"
                    }
                }
            }
        }

        stage('Archive Package') {
            steps {
                archiveArtifacts artifacts: "${PACKAGE_DIR}/**/*", allowEmptyArchive: true
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


