pipeline {
    agent any

    environment {
        PACKAGE_DIR = 'node_package'
        BLN_PACKAGE_NAME = 'bln_node_package.el'
    }

    stages {
        stage('Checkout SCM') {
            steps {
                checkout scm
            }
        }

        stage('Build Node Package') {
            steps {
                script {
                    echo "Running Windows batch build script..."
                    bat 'call build_el_package.bat'
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
