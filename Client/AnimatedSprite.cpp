#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(sf::Time frameTime, bool paused, bool looped) :
    animation(nullptr), frameTime(frameTime), currentFrame(0), isPaused(paused), isLooping(looped), texture(nullptr)
{

}

void AnimatedSprite::setAnimation(const Animation& animation)
{
    this->animation = &animation;
    texture = this->animation->getSpriteSheet();
    currentFrame = 0;
    setFrame(currentFrame);
}

void AnimatedSprite::setFrameTime(sf::Time time)
{
    frameTime = time;
}

void AnimatedSprite::play()
{
    isPaused = false;
}

void AnimatedSprite::play(const Animation& animation)
{
    if (getAnimation() != &animation)
        setAnimation(animation);
    play();
}

void AnimatedSprite::pause()
{
    isPaused = true;
}

void AnimatedSprite::stop()
{
    isPaused = true;
    currentFrame = 0;
    setFrame(currentFrame);
}

void AnimatedSprite::setLooped(bool looped)
{
    isLooping = looped;
}

void AnimatedSprite::setColor(const sf::Color& color)
{
    vertices[0].color = color;
    vertices[1].color = color;
    vertices[2].color = color;
    vertices[3].color = color;
}

const Animation* AnimatedSprite::getAnimation() const
{
    return animation;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
    sf::IntRect rect = animation->getFrame(currentFrame);

    float width = static_cast<float>(std::abs(rect.width));
    float height = static_cast<float>(std::abs(rect.height));

    return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

bool AnimatedSprite::isLooped() const
{
    return isLooping;
}

bool AnimatedSprite::isPlaying() const
{
    return !isPaused;
}

sf::Time AnimatedSprite::getFrameTime() const
{
    return frameTime;
}

void AnimatedSprite::setFrame(std::size_t newFrame, bool resetTime)
{
    if (animation)
    {
        sf::IntRect rect = animation->getFrame(newFrame);

        vertices[0].position = sf::Vector2f(0.f, 0.f);
        vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
        vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
        vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

        float left = static_cast<float>(rect.left) + 0.0001f;
        float right = left + static_cast<float>(rect.width);
        float top = static_cast<float>(rect.top);
        float bottom = top + static_cast<float>(rect.height);

        vertices[0].texCoords = sf::Vector2f(left, top);
        vertices[1].texCoords = sf::Vector2f(left, bottom);
        vertices[2].texCoords = sf::Vector2f(right, bottom);
        vertices[3].texCoords = sf::Vector2f(right, top);
    }

    if (resetTime)
        currentTime = sf::Time::Zero;
}

void AnimatedSprite::update(sf::Time deltaTime)
{
    // if not paused and we have a valid animation
    if (!isPaused && animation)
    {
        // add delta time
        currentTime += deltaTime;

        // if current time is bigger then the frame time advance one frame
        if (currentTime >= frameTime)
        {
            // reset time, but keep the remainder
            currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());

            // get next Frame index
            if (currentFrame + 1 < animation->getSize())
                currentFrame++;
            else
            {
                // animation has ended
                currentFrame = 0; // reset to start

                if (!isLooping)
                {
                    isPaused = true;
                }

            }

            // set the current frame, not reseting the time
            setFrame(currentFrame, false);
        }
    }
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (animation && texture)
    {
        states.transform *= getTransform();
        states.texture = texture;
        target.draw(vertices, 4, sf::Quads, states);
    }
}