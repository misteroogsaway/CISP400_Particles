#include "../include/particles.h"
#include <math.h>

Particle::Particle(sf::RenderTarget& target, int num_points, sf::Vector2i position)
: m_matrix(2, num_points) {

    m_ttl = TTL;
    m_num_points = num_points;
    m_radians_per_sec = static_cast<float>(rand()) / (RAND_MAX);
    m_radians_per_sec *= M_PI;
    m_cartesian_plane.setCenter(0.0, 0.0);
    m_cartesian_plane.setSize(target.getSize().x, (-1.0) * target.getSize().y);
    m_center_coordinate = target.mapPixelToCoords(position, m_cartesian_plane);

    /* 401 comes from (500 - 100 + 1)  to generate a number in the range [100, 500] */
    m_vx = 100 + (rand() % 401);
    m_vy = 100 + (rand() % 401);

    if (rand() % 2 != 0) {
        m_vx *= -1;
    }

    m_cPrimary = sf::Color::White;
    m_cSecondary = sf::Color(rand() % 256, rand() % 256, rand() % 256);

    float theta = (static_cast<float>(rand()) / RAND_MAX) * (M_PI / 2),
          dTheta = 2 * M_PI / (num_points - 1);

    for (int j = 0; j < num_points; j++) {
        /* 61 comes from (80 - 20 + 1) to generate a number in the range [20, 80] */
        float r = 20 + (rand() % 61),
              dx = r * std::cos(theta),
              dy = r * std::sin(theta);

        m_matrix(0, j) = m_center_coordinate.x + dx;
        m_matrix(1, j) = m_center_coordinate.y + dy;

        theta += dTheta;
    }
}

void Particle::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::VertexArray lines(sf::TriangleFan, m_num_points + 1);

    /* I think this is the weirdest thing i've done, but the project specs say to make this a sf::Vector2f */
    sf::Vector2f center = sf::Vector2f(target.mapCoordsToPixel(m_center_coordinate, m_cartesian_plane));

    /* I see why the line above needs to be a Vector2f now... */
    lines[0].position = center;
    lines[0].color = m_cPrimary;;

    for (int j = 1; j <= m_num_points; j++) {
        lines[j].color = m_cSecondary;

        sf::Vector2f column_coordinate = sf::Vector2f(m_matrix(0, j - 1), m_matrix(1, j - 1));
        lines[j].position = sf::Vector2f(target.mapCoordsToPixel(column_coordinate, m_cartesian_plane));
    }

    target.draw(lines);
}

void Particle::update(float dt) {
    m_ttl -= dt;
    rotate(dt * m_radians_per_sec);
    scale(SCALAR);

    m_vy -= GRAVITY * dt;

    float dx = m_vx * dt,
          dy = m_vy * dt;

    translate(dx, dy);
}

void Particle::translate(double x_shift, double y_shift) {
    Matrices::TranslationMatrix T(x_shift, y_shift, m_matrix.get_cols());
    m_matrix = T + m_matrix;


    m_center_coordinate.x += x_shift;
    m_center_coordinate.y += y_shift;
}

void Particle::rotate(double theta) {
    sf::Vector2f temp = m_center_coordinate;

    translate(-m_center_coordinate.x, -m_center_coordinate.y);

    Matrices::RotationMatrix R(theta);
    m_matrix = R * m_matrix;
    translate(temp.x, temp.y);
}

void Particle::scale(double scalar) {
    sf::Vector2f temp = m_center_coordinate;

    translate(-m_center_coordinate.x, -m_center_coordinate.y);

    Matrices::ScalingMatrix S(scalar);
    m_matrix = S * m_matrix;
    translate(temp.x, temp.y);
}

bool Particle::almost_equal(double a, double b, double epsilon) {
    return std::fabs(a - b) < epsilon;
}

void Particle::unit_tests()
{
    int score = 0;

    std::cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    Matrices::RotationMatrix r(M_PI / 4);
    if (r.get_rows() == 2 && r.get_cols() == 2 && almost_equal(r(0, 0), std::cos(theta))
        && almost_equal(r(0, 1), -std::sin(theta))
        && almost_equal(r(1, 0), std::sin(theta))
        && almost_equal(r(1, 1), std::cos(theta)))
    {
        std::cout << "Passed.  +1" << std::endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << std::endl;
    }

    std::cout << "Testing ScalingMatrix constructor...";
    Matrices::ScalingMatrix s(1.5);
    if (s.get_rows() == 2 && s.get_cols() == 2
        && almost_equal(s(0, 0), 1.5)
        && almost_equal(s(0, 1), 0)
        && almost_equal(s(1, 0), 0)
        && almost_equal(s(1, 1), 1.5))
    {
        std::cout << "Passed.  +1" << std::endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << std::endl;
    }

    std::cout << "Testing TranslationMatrix constructor...";
    Matrices::TranslationMatrix t(5, -5, 3);
    if (t.get_rows() == 2 && t.get_cols() == 3
        && almost_equal(t(0, 0), 5)
        && almost_equal(t(1, 0), -5)
        && almost_equal(t(0, 1), 5)
        && almost_equal(t(1, 1), -5)
        && almost_equal(t(0, 2), 5)
        && almost_equal(t(1, 2), -5))
    {
        std::cout << "Passed.  +1" << std::endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << std::endl;
    }


    std::cout << "Testing Particles..." << std::endl;
    std::cout << "Testing Particle mapping to Cartesian origin..." << std::endl;
    if (m_center_coordinate.x != 0 || m_center_coordinate.y != 0)
    {
        std::cout << "Failed.  Expected (0,0).  Received: (" << m_center_coordinate.x << "," << m_center_coordinate.y << ")" << std::endl;
    }
    else
    {
        std::cout << "Passed.  +1" << std::endl;
        score++;
    }

    std::cout << "Applying one rotation of 90 degrees about the origin..." << std::endl;
    Matrices::Matrix initialCoords = m_matrix;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.get_cols(); j++)
    {
        if (!almost_equal(m_matrix(0, j), -initialCoords(1, j)) || !almost_equal(m_matrix(1, j), initialCoords(0, j)))
        {
            std::cout << "Failed mapping: ";
            std::cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_matrix(0, j) << ", " << m_matrix(1, j) << ")" << std::endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        std::cout << "Passed.  +1" << std::endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << std::endl;
    }

    std::cout << "Applying a scale of 0.5..." << std::endl;
    initialCoords = m_matrix;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.get_cols(); j++)
    {
        if (!almost_equal(m_matrix(0, j), 0.5 * initialCoords(0,j)) || !almost_equal(m_matrix(1, j), 0.5 * initialCoords(1, j)))
        {
            std::cout << "Failed mapping: ";
            std::cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_matrix(0, j) << ", " << m_matrix(1, j) << ")" << std::endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        std::cout << "Passed.  +1" << std::endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << std::endl;
    }

    std::cout << "Applying a translation of (10, 5)..." << std::endl;
    initialCoords = m_matrix;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.get_cols(); j++)
    {
        if (!almost_equal(m_matrix(0, j), 10 + initialCoords(0, j)) || !almost_equal(m_matrix(1, j), 5 + initialCoords(1, j)))
        {
            std::cout << "Failed mapping: ";
            std::cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_matrix(0, j) << ", " << m_matrix(1, j) << ")" << std::endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        std::cout << "Passed.  +1" << std::endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << std::endl;
    }

    std::cout << "Score: " << score << " / 7" << std::endl;
}
