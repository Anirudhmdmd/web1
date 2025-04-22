pipeline {
    agent any

    stages {
        stage('Build BLN Node Package') {
            steps {
                script {
                    bat 'build_el_package.bat'
                }
            }
        }

        stage('Archive Package') {
            steps {
                archiveArtifacts artifacts: 'node_package/**/*', allowEmptyArchive: false
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
            echo 'Cleaning up after pipeline execution'
        }
        failure {
            echo 'Pipeline failed'
        }
    }
}
